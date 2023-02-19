
#ifndef SLANG_PRINTER_H
#define SLANG_PRINTER_H

#include "IVisitor.h"
#include <iostream>

namespace slang
{

class Printer : public AbstractVisitor<void, void>
{
public:
    explicit Printer(std::ostream &os, int indent_size = 4);
    void *visit_type(Program &program, void *c) override;
    void *visit_type(FuncDef &def, void *c) override;
    void *visit_type(InferredDeclExpr &expr, void *c) override;
    void *visit_type(ExprStmt &stmt, void *c) override;
    void *visit_type(FuncSignature &signature, void *c) override;
    void *visit_type(AssignExpr &expr, void *c) override;
    void *visit_type(FuncParamDef &def, void *c) override;
    void *visit_type(StmtBlock &list, void *c) override;
    void *visit_type(TypeRef &type, void *c) override;
    void *visit_type(VarRef &var, void *c) override;
    void *visit_type(BinaryExpr &expr, void *c) override;
    void *visit_type(UnaryExpr &expr, void *c) override;
    void *visit_type(VarRefExpr &expr, void *c) override;
    void *visit_type(DerefExpr &expr, void *c) override;
    void *visit_type(LiteralExpr &expr, void *c) override;
    void *visit_type(IfStmt &stmt, void *pVoid) override;
    void *visit_type(ReturnStmt &stmt, void *ctx) override;
    void *visit_type(CallExpr &expr, void *c) override;
    void *visit_type(WhileStmt &stmt, void *c) override;

private:
    std::ostream &indent();

private:
    std::ostream &_os;
    int _indent_level;
    std::string _indent_str;
};

} // namespace slang

#endif //SLANG_PRINTER_H
