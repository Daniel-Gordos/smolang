#ifndef SLANG_IRGENERATOR_H
#define SLANG_IRGENERATOR_H

#include "Exception.h"
#include "visitors/IVisitor.h"
#include <map>
#include <memory>
#include <string>

namespace llvm
{
class Value;
class Type;
class BasicBlock;
} // namespace llvm

namespace slang
{

class Token;
struct FuncContext;
class Type;

using TypeMap = std::map<std::string, std::unique_ptr<Type>, std::less<>>;

class IRGeneratorException : public Exception
{
public:
    explicit IRGeneratorException(std::string);
    [[nodiscard]] const char *what() const noexcept override;

private:
    std::string _msg;
};

class IRGenerator : public AbstractVisitor<FuncContext, llvm::Value>
{
public:
    explicit IRGenerator(TypeMap &, bool);
    ~IRGenerator();

    void print_ir() const;

    llvm::Value *visit_type(Program &program, FuncContext *c) override;
    llvm::Value *visit_type(FuncDefList &list, FuncContext *c) override;
    llvm::Value *visit_type(FuncSignature &signature, FuncContext *c) override;
    llvm::Value *visit_type(BinaryExpr &expr, FuncContext *c) override;
    llvm::Value *visit_type(VarRefExpr &expr, FuncContext *c) override;
    llvm::Value *visit_type(LiteralExpr &expr, FuncContext *c) override;
    llvm::Value *visit_type(FuncDef &def, FuncContext *c) override;
    llvm::Value *visit_type(FuncParamDef &def, FuncContext *c) override;
    llvm::Value *visit_type(FuncParamDefList &list, FuncContext *c) override;
    llvm::Value *visit_type(FuncBody &body, FuncContext *c) override;
    llvm::Value *visit_type(StmtList &list, FuncContext *c) override;
    llvm::Value *visit_type(AssignExpr &expr, FuncContext *c) override;
    llvm::Value *visit_type(InferredDeclExpr &expr, FuncContext *c) override;
    llvm::Value *visit_type(ExprStmt &stmt, FuncContext *c) override;
    llvm::Value *visit_type(ReturnStmt &stmt, FuncContext *c) override;
    llvm::Value *visit_type(CallExpr &expr, FuncContext *c) override;
    llvm::Value *visit_type(IfStmt &stmt, FuncContext *c) override;
    llvm::Value *visit_type(WhileStmt &stmt, FuncContext *c) override;

private:
    llvm::Type *primitive_to_llvm_type(const Type *type);
    void forward_declare_funcs(Program &program);
    llvm::Value *get_float_binary_op(llvm::Value *lhs, llvm::Value *rhs, const Token &op);
    llvm::Value *get_int_binary_op(llvm::Value *lhs, llvm::Value *rhs, const Token &op);
    llvm::Value *get_bool_binary_op(llvm::Value *lhs, llvm::Value *rhs, const Token &op);
    void terminate_block(FuncContext *);
    llvm::BasicBlock *enter_loop_context(FuncContext *);
    void leave_loop_context(FuncContext *);

    struct Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace slang

#endif //SLANG_IRGENERATOR_H
