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

class TypeChecker : public AbstractVisitor<Type, Type>
{
public:
    TypeChecker();

    Type *visit_type(Program &program, Type *c) override;
    Type *visit_type(FuncDefList &list, Type *c) override;
    Type *visit_type(FuncSignature &signature, Type *c) override;
    Type *visit_type(FuncParamDef &def, Type *c) override;
    Type *visit_type(FuncParamDefList &list, Type *c) override;
    Type *visit_type(FuncBody &body, Type *c) override;
    Type *visit_type(FuncDef &def, Type *c) override;
    Type *visit_type(StmtList &list, Type *c) override;
    Type *visit_type(VarRef &var, Type *c) override;
    Type *visit_type(CallExpr &expr, Type *c) override;
    Type *visit_type(CallArgList &list, Type *c) override;
    Type *visit_type(ExprStmt &stmt, Type *c) override;
    Type *visit_type(BinaryExpr &expr, Type *c) override;
    Type *visit_type(UnaryExpr &expr, Type *c) override;
    Type *visit_type(VarRefExpr &expr, Type *c) override;
    Type *visit_type(InferredDeclExpr &expr, Type *c) override;
    Type *visit_type(LiteralExpr &expr, Type *c) override;
    Type *visit_type(AssignExpr &expr, Type *c) override;
    Type *visit_type(ReturnStmt &stmt, Type *c) override;
    Type *visit_type(IfStmt &stmt, Type *c) override;
    Type *visit_type(WhileStmt &stmt, Type *c) override;

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
    void verify_func_call_args(const FuncSignature &, CallExpr &);
    void initialize_builtin_funcs();

    SymbolTable _symbol_table;
};

} // namespace slang

#endif //SLANG_TYPECHECKER_H
