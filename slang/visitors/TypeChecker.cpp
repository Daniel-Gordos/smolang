#include "TypeChecker.h"

#include "Program.h"
#include "Token.h"
#include "TypeRef.h"
#include "VarRef.h"
#include "expressions/AssignExpr.h"
#include "expressions/BinaryExpr.h"
#include "expressions/CallExpr.h"
#include "expressions/DerefExpr.h"
#include "expressions/InferredDeclExpr.h"
#include "expressions/LiteralExpr.h"
#include "expressions/UnaryExpr.h"
#include "expressions/VarRefExpr.h"
#include "functions/FuncBody.h"
#include "functions/FuncSignature.h"
#include "statements/ExprStmt.h"
#include "statements/IfStmt.h"
#include "statements/ReturnStmt.h"
#include "statements/WhileStmt.h"
#include <sstream>
#include <unordered_set>

namespace slang
{

TypeCheckerException::TypeCheckerException(std::string msg)
    : _msg(std::move(msg))
{}

const char *TypeCheckerException::what() const noexcept { return _msg.c_str(); }

TypeCheckerException
make_redefinition_error(const VarRef &original, const VarRef &redefinition)
{
    auto buf = std::stringstream{};
    buf << redefinition.get_token().get_pos() << " : Redefinition of symbol \""
        << redefinition.get_token().get_spelling() << "\" from "
        << original.get_token().get_pos();
    return TypeCheckerException{buf.str()};
}

TypeCheckerException make_not_found_error(const Token &tok)
{
    auto buf = std::stringstream{};
    buf << tok.get_pos() << " : Symbol \"" << tok.get_spelling() << "\" not found";
    return TypeCheckerException{buf.str()};
}

TypeCheckerException make_not_assignable_error(const Type &lhs, const Type &rhs)
{
    auto buf = std::stringstream{};
    buf << "Cannot assign expression of type " << rhs.get_name() << " to "
        << lhs.get_name();
    return TypeCheckerException(buf.str());
}

TypeCheckerException make_expr_not_bool_error()
{
    return TypeCheckerException("Control flow expression must be assignable as bool");
}

TypeCheckerException
make_binary_incompatible_error(const Type &lhs, const Type &rhs, const Token &op)
{
    auto buf = std::stringstream{};
    buf << "Types " << lhs.get_name() << " and " << rhs.get_name()
        << " are incompatible with operator " << op.get_spelling();
    return TypeCheckerException(buf.str());
}

TypeCheckerException make_unary_incompatible_error(const Type &type, const Token &op)
{
    auto buf = std::stringstream{};
    buf << "Type " << type.get_name() << " is incompatible with unary operator "
        << op.get_spelling();
    return TypeCheckerException(buf.str());
}

TypeCheckerException make_wrong_arg_count_error(CallExpr &expr)
{
    auto func_name = expr.get_name()->get_token();
    auto buf = std::stringstream{};
    buf << "Wrong number of arguments passed to " << func_name.get_spelling() << " at "
        << func_name.get_pos();
    return TypeCheckerException(buf.str());
}

bool TypeChecker::is_assignable_to(Type &lhs, Type &rhs) const
{
    return rhs.is_assignable_from(&lhs);
}

bool TypeChecker::is_convertable_to(Type &from, Type &to)
{
    return from.is_assignable_from(&to);
}

bool TypeChecker::is_binary_op_compatible(Type &lhs, Type &rhs, const Token &op)
{
    return lhs.is_binary_op_compatible(&rhs, op);
}

bool TypeChecker::is_unary_op_compatible(Type &type, const Token &op)
{
    return type.is_unary_op_compatible(op);
}

Type *TypeChecker::get_literal_type(const Token &token)
{
    switch (token.get_kind())
    {
        case Token::INT_LIT:
            return _symbol_table.retrieve_type("int");
        case Token::FLOAT_LIT:
            return _symbol_table.retrieve_type("float");
        case Token::BOOL_LIT:
            return _symbol_table.retrieve_type("bool");
        case Token::STR_LIT:
            return _symbol_table.retrieve_type("str");
        default:
            throw TypeCheckerException("Unexpected literal type");
    }
}
void TypeChecker::register_funcs(Program &program)
{

    for (auto def_list = program.func_def_list.get(); def_list;
         def_list = def_list->get_next().get())
    {
        auto &func = def_list->get_func_def();
        _symbol_table.enter_func(*func->get_signature());
    }
}

void TypeChecker::add_to_symbol_table(VarRef &var, TypeRef &var_type)
{
    auto existing = _symbol_table.retrieve_var(var.get_token().get_spelling());
    if (existing)
        throw make_redefinition_error(*existing->ref, var);
    auto existing_type = _symbol_table.retrieve_type(var_type.get_token().get_spelling());
    if (!existing_type)
        throw make_not_found_error(var_type.get_token());
    _symbol_table.enter_var(var, existing_type);
}

TypeChecker::TypeChecker()
    : _symbol_table{}
{}

void check_if_func_names_unique(Program &program)
{
    auto func_names = std::unordered_set<std::string_view>{};
    for (auto def_list = program.func_def_list.get(); def_list;
         def_list = def_list->get_next().get())
    {
        auto &func = def_list->get_func_def();
        auto name = func->get_signature()->get_name()->get_token().get_spelling();
        if (func_names.contains(name))
        {
            auto buf = std::stringstream{} << "Multiple definitions for function \""
                                           << name << "\"";
            throw TypeCheckerException(buf.str());
        }
        func_names.insert(name);
    }
}

void check_has_main_func(Program &program)
{
    for (auto def_list = program.func_def_list.get(); def_list;
         def_list = def_list->get_next().get())
    {
        auto &func_signature = def_list->get_func_def()->get_signature();
        auto name = func_signature->get_name()->get_token().get_spelling();

        if (name == "main")
        {
            if (func_signature->get_params())
                throw TypeCheckerException("Main function should have no arguments");
            return;
        }
    }
    throw TypeCheckerException("Main function not found");
}
Type *TypeChecker::visit_type(WhileStmt &stmt, Type *c)
{
    auto expr_type = static_cast<Type *>(stmt.get_expr()->accept(*this, c));
    if (!is_assignable_to(*_symbol_table.retrieve_type("bool"), *expr_type))
        throw make_expr_not_bool_error();

    if (stmt.get_stmts())
    {
        _symbol_table.open_scope();
        stmt.get_stmts()->accept(*this, c);
        _symbol_table.close_scope();
    }

    return {};
}
Type *TypeChecker::get_unary_op_type(Type &type, const Token &op)
{
    if (op.get_kind() == Token::PLUS || op.get_kind() == Token::MINUS ||
        op.get_kind() == Token::MINUS || op.get_kind() == Token::DIV)
    {
        return &type;
    }
    return _symbol_table.retrieve_type("bool");
}

Type *TypeChecker::get_binary_op_type(Type &lhs, Type &rhs, const Token &op)
{
    auto kind = op.get_kind();
    if (kind == Token::EQEQ || kind == Token::NOTEQ || kind == Token::GE ||
        kind == Token::GT || kind == Token::LE || kind == Token::LT)
        return _symbol_table.retrieve_type("bool");

    return &rhs;
}

Type *TypeChecker::visit_type(Program &program, Type *c)
{
    check_if_func_names_unique(program);
    check_has_main_func(program);
    register_funcs(program);

    program.func_def_list->accept(*this, {});

    return {};
}

Type *TypeChecker::visit_type(FuncDefList &list, Type *c)
{
    list.get_func_def()->accept(*this, c);
    if (list.get_next())
        list.get_next()->accept(*this, c);
    return {};
}

Type *TypeChecker::visit_type(FuncDef &def, Type *c)
{
    _symbol_table.open_scope();
    def.get_signature()->accept(*this, c);
    def.get_body()->accept(*this, c);
    _symbol_table.close_scope();
    return {};
}

Type *TypeChecker::visit_type(FuncSignature &signature, Type *c)
{
    _symbol_table.enter_func(signature);
    if (signature.get_params())
        signature.get_params()->accept(*this, c);

    if (signature.get_return_type())
    {
        auto token = signature.get_return_type()->get_token();
        auto type = _symbol_table.retrieve_type(token.get_spelling());
        if (!type)
            throw make_not_found_error(token);
        signature.get_return_type()->set_type(type);
    }

    return {};
}

Type *TypeChecker::visit_type(IfStmt &stmt, Type *c)
{
    auto expr_type = static_cast<Type *>(stmt.get_expr()->accept(*this, c));
    if (!is_assignable_to(*_symbol_table.retrieve_type("bool"), *expr_type))
        throw make_expr_not_bool_error();

    if (stmt.get_stmts())
    {
        _symbol_table.open_scope();
        stmt.get_stmts()->accept(*this, c);
        _symbol_table.close_scope();
    }

    if (stmt.get_else_stmts())
        stmt.get_else_stmts()->accept(*this, c);

    return {};
}

Type *TypeChecker::visit_type(FuncParamDef &def, Type *c)
{
    auto type_name = def.get_type()->get_token().get_spelling();
    auto type = _symbol_table.retrieve_type(type_name);
    if (!type)
        throw make_not_found_error(def.get_type()->get_token());
    _symbol_table.enter_var(*def.get_var(), type);
    def.get_type()->set_type(type);
    return type;
}

Type *TypeChecker::visit_type(FuncParamDefList &list, Type *c)
{
    list.get_param()->accept(*this, c);
    if (list.get_next())
        list.get_next()->accept(*this, c);
    return {};
}

Type *TypeChecker::visit_type(FuncBody &body, Type *c)
{
    if (body.get_statements())
        body.get_statements()->accept(*this, c);
    return {};
}

TypeMap TypeChecker::get_defined_types() { return _symbol_table.acquire_types(); }

Type *TypeChecker::visit_type(StmtList &list, Type *c)
{
    list.get_statement()->accept(*this, c);
    if (list.get_next())
        list.get_next()->accept(*this, c);
    return {};
}

Type *TypeChecker::visit_type(VarRef &var, Type *c)
{
    auto definition = _symbol_table.retrieve_var(var.get_token().get_spelling());
    if (!definition)
        make_not_found_error(var.get_token());
    return definition->type;
}

Type *TypeChecker::visit_type(ExprStmt &stmt, Type *c)
{
    stmt.get_expr()->accept(*this, c);
    return {};
}

Type *TypeChecker::visit_type(BinaryExpr &expr, Type *c)
{
    auto lhs_type = static_cast<Type *>(expr.get_lhs()->accept(*this, c));
    auto rhs_type = static_cast<Type *>(expr.get_rhs()->accept(*this, c));
    if (!is_binary_op_compatible(*lhs_type, *rhs_type, expr.get_op()))
        throw make_binary_incompatible_error(*lhs_type, *rhs_type, expr.get_op());

    auto synthed_type = get_binary_op_type(*lhs_type, *rhs_type, expr.get_op());
    expr.set_type(synthed_type);
    return synthed_type;
}

Type *TypeChecker::visit_type(UnaryExpr &expr, Type *c)
{
    auto expr_type = static_cast<Type *>(expr.get_expr()->accept(*this, c));
    auto &op = expr.get_op();
    if (!is_unary_op_compatible(*expr_type, op))
        throw make_unary_incompatible_error(*expr_type, op);
    auto synthed_type = get_unary_op_type(*expr_type, op);
    expr.set_type(synthed_type);
    return synthed_type;
}

Type *TypeChecker::visit_type(VarRefExpr &expr, Type *c)
{
    auto existing =
        _symbol_table.retrieve_var(expr.get_var()->get_token().get_spelling());
    if (!existing)
        throw make_not_found_error(expr.get_var()->get_token());
    expr.set_type(existing->type);

    return existing->type;
}

Type *TypeChecker::visit_type(ReturnStmt &stmt, Type *c)
{
    stmt.get_expr()->accept(*this, c);
    return {};
}

Type *TypeChecker::visit_type(LiteralExpr &expr, Type *c)
{
    auto type = get_literal_type(expr.get_token());
    expr.set_type(type);
    return type;
}

Type *TypeChecker::visit_type(AssignExpr &expr, Type *c)
{
    auto lhs_type = static_cast<Type *>(expr.get_lhs()->accept(*this, c));
    auto rhs_type = static_cast<Type *>(expr.get_rhs()->accept(*this, c));

    if (!is_assignable_to(*lhs_type, *rhs_type))
        throw make_not_assignable_error(*lhs_type, *rhs_type);

    expr.set_type(rhs_type);
    return rhs_type;
}

Type *TypeChecker::visit_type(InferredDeclExpr &expr, Type *c)
{
    auto &new_var = expr.get_lhs()->get_var();
    auto var_name = new_var->get_token().get_spelling();

    auto type = static_cast<Type *>(expr.get_rhs()->accept(*this, c));
    if (auto existing = _symbol_table.retrieve_var(var_name))
        throw make_redefinition_error(*existing->ref, *new_var);
    _symbol_table.enter_var(*new_var, type);

    expr.get_lhs()->set_type(type);
    expr.set_type(type);

    return type;
}

Type *TypeChecker::visit_type(CallExpr &expr, Type *c)
{
    auto func_name = expr.get_name()->get_token().get_spelling();
    auto declaration = _symbol_table.retrieve_func(func_name);
    if (!declaration)
        throw make_not_found_error(expr.get_name()->get_token());

    auto return_type = declaration->get_return_type()->get_type();
    expr.set_type(return_type);

    if (expr.get_args())
        expr.get_args()->accept(*this, c);

    verify_func_call_args(*declaration, expr);

    return return_type;
}

Type *TypeChecker::visit_type(CallArgList &list, Type *c)
{
    list.get_arg()->accept(*this, c);
    if (list.get_next())
        list.get_next()->accept(*this, c);

    return {};
}

void TypeChecker::verify_func_call_args(const FuncSignature &signature, CallExpr &expr)
{
    auto signature_node = signature.get_params().get();
    auto call_node = expr.get_args().get();
    while (signature_node && call_node)
    {
        auto expected_type = signature_node->get_param()->get_type()->get_type();
        auto got_type = call_node->get_arg()->get_type();
        if (!is_assignable_to(*expected_type, *got_type))
            throw make_not_assignable_error(*expected_type, *got_type);

        signature_node = signature_node->get_next().get();
        call_node = call_node->get_next().get();
    }
    if (signature_node || call_node)
        throw make_wrong_arg_count_error(expr);
}

void TypeChecker::initialize_builtin_funcs()
{
    auto putint_args = std::make_unique<FuncParamDefList>(
        std::make_unique<FuncParamDef>(
            std::make_unique<TypeRef>(Token(Token::IDENT, {}, "int")),
            std::make_unique<VarRef>(Token(Token::IDENT, {}, "i"))),
        nullptr);

    auto putint = std::make_unique<FuncSignature>(
        nullptr, std::make_unique<FuncRef>(Token(Token::IDENT, {}, "putint")),
        std::move(putint_args));
}

} // namespace slang