#include "IrGenerator.h"
#include "Program.h"
#include "Type.h"
#include "expressions/AssignExpr.h"
#include "expressions/BinaryExpr.h"
#include "expressions/CallExpr.h"
#include "expressions/InferredDeclExpr.h"
#include "expressions/LiteralExpr.h"
#include "expressions/UnaryExpr.h"
#include "expressions/VarRefExpr.h"
#include "statements/ExprStmt.h"
#include "statements/IfStmt.h"
#include "statements/ReturnStmt.h"
#include "statements/StmtBlock.h"
#include "statements/WhileStmt.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include <map>
#include <sstream>
#include <string>

namespace slang
{

class Type;

struct IRGenerator::Impl
{
    Impl(TypeMap &type_map, bool run_optimizations)
        : context(llvm::LLVMContext())
        , builder(llvm::IRBuilder(context))
        , module(llvm::Module("Main", context))
        , type_map(type_map)
    {}

    llvm::LLVMContext context;
    llvm::IRBuilder<> builder;
    llvm::Module module;
    TypeMap &type_map;
};

IRGenerator::IRGenerator(TypeMap &type_map, bool run_optimizations = true)
    : _impl(std::make_unique<Impl>(type_map, run_optimizations))
{}

IRGeneratorException make_local_not_found_error(const Token &token)
{
    auto buf = std::stringstream{};
    buf << token.get_pos() << ": Failed to find var \"" << token.get_spelling() << "\"";
    return IRGeneratorException(buf.str());
}

struct FuncContext
{
    FuncSignature &signature;
    llvm::Function &func;
    llvm::BasicBlock *hoist_block;
    int loop_depth = 0;
    std::map<std::string, llvm::AllocaInst *, std::less<>> locals = {};
};

void IRGenerator::forward_declare_funcs(Program &program)
{
    for (auto &func: program)
    {
        auto &func_signature = func->get_signature();
        auto name = func_signature->get_name()->get_token().get_spelling();

        // Construct list of parameters
        auto params = std::vector<llvm::Type *>{};
        for (auto &param: *func_signature)
        {
            auto type = primitive_to_llvm_type(param->get_type()->get_type());
            params.push_back(type);
        }

        auto return_type = func_signature->get_return_type()
                               ? func_signature->get_return_type()->get_type()
                               : nullptr;
        auto llvm_return_type = primitive_to_llvm_type(return_type);
        auto llvm_signature = llvm::FunctionType::get(llvm_return_type, params, false);

        auto llvm_func = llvm::Function::Create(
            llvm_signature, llvm::GlobalValue::ExternalLinkage, name, _impl->module);

        // Set parameter names
        auto arg_it = llvm_func->arg_begin();
        for (auto param_it = func_signature->begin(); param_it != func_signature->end();
             ++arg_it, ++param_it)
        {
            auto &arg_name = param_it->get()->get_var()->get_token();
            arg_it->setName(arg_name.get_spelling());
        }
    }
}

llvm::Value *IRGenerator::visit_type(Program &program, FuncContext *c)
{
    forward_declare_funcs(program);

    for (auto &func: program)
        func->accept(*this, c);

    return {};
}
llvm::Value *IRGenerator::visit_type(FuncDef &def, FuncContext *c)
{
    auto name = def.get_signature()->get_name()->get_token().get_spelling();
    auto func = _impl->module.getFunction(name);

    auto entry_block = llvm::BasicBlock::Create(_impl->context, "entry", func);
    _impl->builder.SetInsertPoint(entry_block);

    auto ctx = FuncContext{*def.get_signature(), *func};

    def.get_signature()->accept(*this, &ctx);
    if (def.get_body())
        def.get_body()->accept(*this, &ctx);
    terminate_block(&ctx);
    return {};
}

llvm::Value *IRGenerator::visit_type(FuncSignature &signature, FuncContext *c)
{

    auto i = 0;
    for (auto &param: signature)
    {
        auto arg = c->func.getArg(i);

        auto var_name = param->get_var()->get_token().get_spelling();

        auto llvm_type = primitive_to_llvm_type(param->get_type()->get_type());
        auto alloc = _impl->builder.CreateAlloca(llvm_type, {}, var_name);
        _impl->builder.CreateStore(arg, alloc);

        c->locals[std::string(var_name)] = alloc;
    }

    return {};
}

llvm::Value *IRGenerator::visit_type(FuncParamDef &def, FuncContext *c) { return {}; }

void IRGenerator::print_ir() const { _impl->module.print(llvm::outs(), nullptr); }

llvm::Value *IRGenerator::visit_type(BinaryExpr &expr, FuncContext *c)
{
    auto lhs = static_cast<llvm::Value *>(expr.get_lhs()->accept(*this, c));
    auto rhs = static_cast<llvm::Value *>(expr.get_rhs()->accept(*this, c));

    auto type_name = expr.get_type()->get_name();
    if (type_name == "int")
    {
        return get_int_binary_op(lhs, rhs, expr.get_op());
    }
    else if (type_name == "float")
    {
        return get_float_binary_op(lhs, rhs, expr.get_op());
    }
    else
    {
        return get_bool_binary_op(lhs, rhs, expr.get_op());
    }
}

/// Always creates a new block after the return
llvm::Value *IRGenerator::visit_type(ReturnStmt &stmt, FuncContext *c)
{
    auto val = static_cast<llvm::Value *>(stmt.get_expr()->accept(*this, c));
    _impl->builder.CreateRet(val);

    auto curr_func = &c->func;
    auto next_block = llvm::BasicBlock::Create(_impl->context, "", curr_func);

    _impl->builder.SetInsertPoint(next_block);
    return {};
}

llvm::Value *IRGenerator::visit_type(AssignExpr &expr, FuncContext *c)
{
    auto rhs_val = static_cast<llvm::Value *>(expr.get_rhs()->accept(*this, c));

    auto lhs = dynamic_cast<VarRefExpr *>(expr.get_lhs().get());
    auto var_name = lhs->get_var()->get_token().get_spelling();

    auto it = c->locals.find(var_name);
    if (it == c->locals.end())
        throw make_local_not_found_error(lhs->get_var()->get_token());
    auto alloc = it->second;

    _impl->builder.CreateStore(rhs_val, alloc);
    return rhs_val;
}

llvm::Value *IRGenerator::visit_type(VarRefExpr &expr, FuncContext *c)
{
    auto var_token = expr.get_var()->get_token();
    auto it = c->locals.find(var_token.get_spelling());
    if (it == c->locals.end())
        throw make_local_not_found_error(var_token);
    auto alloc = it->second;

    auto val = _impl->builder.CreateLoad(alloc->getAllocatedType(), alloc);
    return val;
}

llvm::Value *IRGenerator::visit_type(LiteralExpr &expr, FuncContext *c)
{
    auto type_name = expr.get_type()->get_name();
    if (type_name == "int")
    {
        auto val = std::stoi(expr.get_token().get_spelling().data());
        return llvm::ConstantInt::get(_impl->context, llvm::APInt(32, val, true));
    }
    else if (type_name == "float")
    {
        auto val = std::stof(expr.get_token().get_spelling().data());
        return llvm::ConstantFP::get(_impl->context, llvm::APFloat(val));
    }
    else
    {
        if (expr.get_token().get_spelling() == "true")
            return llvm::ConstantInt::get(_impl->context, llvm::APInt(8, 1));
        else
            return llvm::ConstantInt::get(_impl->context, llvm::APInt(8, 0));
    }
    // TODO: Error handling on this
}

llvm::Value *IRGenerator::visit_type(CallExpr &expr, FuncContext *c)
{
    auto args = std::vector<llvm::Value *>{};
    for (auto &arg: expr)
    {
        auto res = arg->accept(*this, c);
        args.push_back(static_cast<llvm::Value *>(res));
    }

    auto func_name = expr.get_name()->get_token().get_spelling();
    auto func = _impl->module.getFunction(func_name);

    return _impl->builder.CreateCall(func, args);
}

llvm::Value *
slang::IRGenerator::visit_type(slang::InferredDeclExpr &expr, slang::FuncContext *c)
{
    auto type = expr.get_rhs()->get_type();
    auto val = static_cast<llvm::Value *>(expr.get_rhs()->accept(*this, c));

    auto lhs = static_cast<VarRefExpr *>(expr.get_lhs().get());
    auto var_name = lhs->get_var()->get_token().get_spelling();

    auto llvm_type = primitive_to_llvm_type(type);

    llvm::AllocaInst *alloc;
    // Hoist allocation if in a loop context
    if (c->hoist_block)
    {
        auto curr_pos = _impl->builder.GetInsertBlock();
        _impl->builder.SetInsertPoint(&c->hoist_block->back());
        alloc = _impl->builder.CreateAlloca(llvm_type, {}, var_name);
        _impl->builder.SetInsertPoint(curr_pos);
    }
    else
    {
        alloc = _impl->builder.CreateAlloca(llvm_type, {}, var_name);
    }

    c->locals[std::string(var_name)] = alloc;

    _impl->builder.CreateStore(val, alloc);

    return val;
}

llvm::Value *slang::IRGenerator::visit_type(slang::ExprStmt &stmt, slang::FuncContext *c)
{
    stmt.get_expr()->accept(*this, c);
    return {};
}

/// Fill in the end of a block with a zeroed return value
void IRGenerator::terminate_block(FuncContext *ctx)
{
    auto &type_ref = ctx->signature.get_return_type();
    if (!type_ref)
    {
        _impl->builder.CreateRetVoid();
        return;
    }
    auto type = type_ref->get_type();
    llvm::Value *val;
    if (type->get_name() == "int")
    {
        val = llvm::ConstantInt::get(_impl->context, llvm::APInt(32, 0));
    }
    else if (type->get_name() == "float")
    {
        val = llvm::ConstantFP::get(_impl->context, llvm::APFloat(0.0));
    }
    else
    {
        val = llvm::ConstantInt::get(_impl->context, llvm::APInt(1, 0));
    }

    _impl->builder.CreateRet(val);
}
llvm::Value *
IRGenerator::get_float_binary_op(llvm::Value *lhs, llvm::Value *rhs, const Token &op)
{
    switch (op.get_kind())
    {
        case Token::PLUS:
            return _impl->builder.CreateFAdd(lhs, rhs);
        case Token::MINUS:
            return _impl->builder.CreateFSub(lhs, rhs);
        case Token::MULT:
            return _impl->builder.CreateFMul(lhs, rhs);
        case Token::DIV:
            return _impl->builder.CreateFDiv(lhs, rhs);
    }
}

llvm::Value *IRGenerator::visit_type(IfStmt &stmt, FuncContext *c)
{
    auto val = static_cast<llvm::Value *>(stmt.get_expr()->accept(*this, c));
    auto zero_val = llvm::ConstantInt::get(_impl->context, llvm::APInt(1, 0));
    auto condition = _impl->builder.CreateICmpNE(val, zero_val, "if");

    auto func = &c->func;

    auto then_block = llvm::BasicBlock::Create(_impl->context, "if_then", func);
    auto else_block = llvm::BasicBlock::Create(_impl->context, "if_else");
    auto end_block = llvm::BasicBlock::Create(_impl->context, "if_end");

    _impl->builder.CreateCondBr(condition, then_block, else_block);

    // Then
    _impl->builder.SetInsertPoint(then_block);
    if (stmt.get_stmts())
        stmt.get_stmts()->accept(*this, c);
    _impl->builder.CreateBr(end_block);

    // Else
    func->insert(func->end(), else_block);
    _impl->builder.SetInsertPoint(else_block);
    if (stmt.get_else_stmts())
        stmt.get_else_stmts()->accept(*this, c);
    _impl->builder.CreateBr(end_block);

    // End
    func->insert(func->end(), end_block);
    _impl->builder.SetInsertPoint(end_block);

    return {};
}

llvm::Value *IRGenerator::visit_type(StmtBlock &block, FuncContext *c)
{
    for (auto &stmt: block)
        stmt->accept(*this, c);
    return {};
}

llvm::Value *IRGenerator::visit_type(WhileStmt &stmt, FuncContext *c)
{
    auto func = &c->func;

    enter_loop_context(c);

    auto condition_block =
        llvm::BasicBlock::Create(_impl->context, "while_condition", func);
    _impl->builder.CreateBr(condition_block);
    _impl->builder.SetInsertPoint(condition_block);

    auto val = static_cast<llvm::Value *>(stmt.get_expr()->accept(*this, c));
    auto zero_val = llvm::ConstantInt::get(_impl->context, llvm::APInt(1, 0));
    auto condition = _impl->builder.CreateICmpNE(val, zero_val, "while");

    auto body_block = llvm::BasicBlock::Create(_impl->context, "while_body");
    auto end_block = llvm::BasicBlock::Create(_impl->context, "while_end");

    _impl->builder.CreateCondBr(condition, body_block, end_block);

    // Loop body
    func->insert(func->end(), body_block);
    _impl->builder.SetInsertPoint(body_block);
    if (stmt.get_stmts())
        stmt.get_stmts()->accept(*this, c);
    _impl->builder.CreateBr(condition_block);

    // Loop end
    func->insert(func->end(), end_block);
    _impl->builder.SetInsertPoint(end_block);

    leave_loop_context(c);

    return {};
}

llvm::Value *
IRGenerator::get_int_binary_op(llvm::Value *lhs, llvm::Value *rhs, const Token &op)
{
    switch (op.get_kind())
    {
        case Token::PLUS:
            return _impl->builder.CreateAdd(lhs, rhs);
        case Token::MINUS:
            return _impl->builder.CreateSub(lhs, rhs);
        case Token::MULT:
            return _impl->builder.CreateMul(lhs, rhs);
        case Token::DIV:
            return _impl->builder.CreateSDiv(lhs, rhs);
    }
}
llvm::Type *IRGenerator::primitive_to_llvm_type(const Type *type)
{
    if (!type)
    {
        return llvm::Type::getVoidTy(_impl->context);
    }
    else if (type->get_name() == "int")
    {
        return llvm::Type::getInt32Ty(_impl->context);
    }
    else if (type->get_name() == "float")
    {
        return llvm::Type::getFloatTy(_impl->context);
    }
    else
    {
        return llvm::Type::getInt8Ty(_impl->context);
    }
    // TODO: error handling logic on this
}
llvm::Value *
IRGenerator::get_bool_binary_op(llvm::Value *lhs, llvm::Value *rhs, const Token &op)
{
    auto float_type = llvm::Type::getFloatTy(_impl->context);
    auto bool_type = llvm::Type::getInt8Ty(_impl->context);
    if (lhs->getType() == float_type || rhs->getType() == float_type)
    {
        switch (op.get_kind())
        {
            case Token::EQEQ:
                return _impl->builder.CreateFCmpOEQ(lhs, rhs);
            case Token::NOTEQ:
                return _impl->builder.CreateFCmpONE(lhs, rhs);
            case Token::GT:
                return _impl->builder.CreateFCmpOGT(lhs, rhs);
            case Token::GE:
                return _impl->builder.CreateFCmpOGE(lhs, rhs);
            case Token::LT:
                return _impl->builder.CreateFCmpOLT(lhs, rhs);
            case Token::LE:
                return _impl->builder.CreateFCmpOLE(lhs, rhs);
        }
    }
    else if (lhs->getType() == bool_type || rhs->getType() == bool_type)
    {
        switch (op.get_kind())
        {
            case Token::EQEQ:
                return _impl->builder.CreateICmpEQ(lhs, rhs);
            case Token::NOTEQ:
                return _impl->builder.CreateICmpNE(lhs, rhs);
            case Token::AND:
            {
                auto lhs_bool = _impl->builder.CreateIntCast(
                    lhs, llvm::Type::getInt1Ty(_impl->context), true);
                auto rhs_bool = _impl->builder.CreateIntCast(
                    rhs, llvm::Type::getInt1Ty(_impl->context), true);
                return _impl->builder.CreateLogicalAnd(lhs_bool, rhs_bool);
            }
            case Token::OR:
            {
                auto lhs_bool = _impl->builder.CreateIntCast(
                    lhs, llvm::Type::getInt1Ty(_impl->context), true);
                auto rhs_bool = _impl->builder.CreateIntCast(
                    rhs, llvm::Type::getInt1Ty(_impl->context), true);
                return _impl->builder.CreateLogicalOr(lhs_bool, rhs_bool);
            }
        }
    }
    else
    {
        switch (op.get_kind())
        {
            case Token::EQEQ:
            {
                auto int_1_val = _impl->builder.CreateICmpEQ(lhs, rhs);
                return _impl->builder.CreateIntCast(
                    int_1_val, llvm::Type::getInt8Ty(_impl->context), true);
            }
            case Token::NOTEQ:
            {
                auto int_1_val = _impl->builder.CreateICmpNE(lhs, rhs);
                return _impl->builder.CreateIntCast(
                    int_1_val, llvm::Type::getInt8Ty(_impl->context), true);
            }
            case Token::GT:
                return _impl->builder.CreateICmpSGT(lhs, rhs);
            case Token::GE:
                return _impl->builder.CreateICmpSGE(lhs, rhs);
            case Token::LT:
                return _impl->builder.CreateICmpSLT(lhs, rhs);
            case Token::LE:
                return _impl->builder.CreateICmpSLE(lhs, rhs);
        }
    }
}
llvm::BasicBlock *IRGenerator::enter_loop_context(FuncContext *ctx)
{
    ctx->loop_depth++;
    if (ctx->loop_depth > 1)
        return nullptr;

    ctx->hoist_block =
        llvm::BasicBlock::Create(_impl->context, "loop_hoisting", &ctx->func);
    _impl->builder.CreateBr(ctx->hoist_block);
    _impl->builder.SetInsertPoint(ctx->hoist_block);
    return ctx->hoist_block;
}
void IRGenerator::leave_loop_context(FuncContext *ctx)
{
    ctx->loop_depth--;
    if (ctx->loop_depth == 0)
        ctx->hoist_block = nullptr;
}

slang::IRGenerator::~IRGenerator() = default;

IRGeneratorException::IRGeneratorException(std::string msg)
    : _msg(std::move(msg))
{}

const char *IRGeneratorException::what() const noexcept { return _msg.c_str(); }

} // namespace slang