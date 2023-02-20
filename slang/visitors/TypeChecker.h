#ifndef SLANG_TYPECHECKER_H
#define SLANG_TYPECHECKER_H

#include "Exception.h"
#include "IVisitor.h"
#include "SymbolTable.h"
#include "Type.h"

namespace slang
{

class Token;

class TypeCheckerException : public Exception
{
public:
    explicit TypeCheckerException(std::string);
    [[nodiscard]] const char *what() const noexcept override;

private:
    std::string _msg;
};

struct FuncContext;

class TypeChecker : public AbstractVisitor<FuncContext, Type>
{
public:
    TypeChecker();

    Type *visit_type(Program &program, FuncContext *c) override;
    Type *visit_type(FuncSignature &signature, FuncContext *c) override;
    Type *visit_type(FuncParamDef &def, FuncContext *c) override;
    Type *visit_type(FuncDef &def, FuncContext *c) override;
    Type *visit_type(StmtBlock &list, FuncContext *c) override;
    Type *visit_type(VarRef &var, FuncContext *c) override;
    Type *visit_type(CallExpr &expr, FuncContext *c) override;
    Type *visit_type(ExprStmt &stmt, FuncContext *c) override;
    Type *visit_type(BinaryExpr &expr, FuncContext *c) override;
    Type *visit_type(UnaryExpr &expr, FuncContext *c) override;
    Type *visit_type(VarRefExpr &expr, FuncContext *c) override;
    Type *visit_type(InferredDeclExpr &expr, FuncContext *c) override;
    Type *visit_type(LiteralExpr &expr, FuncContext *c) override;
    Type *visit_type(AssignExpr &expr, FuncContext *c) override;
    Type *visit_type(ReturnStmt &stmt, FuncContext *c) override;
    Type *visit_type(IfStmt &stmt, FuncContext *c) override;
    Type *visit_type(WhileStmt &stmt, FuncContext *c) override;

    TypeMap get_defined_types();

private:
    void register_funcs(Program &);
    void add_to_symbol_table(VarRef &, TypeRef &);
    bool is_assignable_to(Type &, Type &) const;
    bool is_convertable_to(Type &, Type &);
    bool is_binary_op_compatible(Type &, Type &, const Token &);
    bool is_unary_op_compatible(Type &, const Token &);
    Type *get_unary_op_type(Type &, const Token &);
    Type *get_binary_op_type(Type &, Type &, const Token &);
    Type *get_literal_type(const Token &);
    void verify_func_call_args(FuncSignature &, CallExpr &);
    void initialize_builtin_funcs();

    SymbolTable _symbol_table;
};

} // namespace slang

#endif //SLANG_TYPECHECKER_H
