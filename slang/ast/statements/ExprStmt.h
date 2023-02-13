#ifndef SLANG_EXPRSTMT_H
#define SLANG_EXPRSTMT_H
#include "Stmt.h"
#include "expressions/Expr.h"

namespace slang
{

class ExprStmt : public Stmt
{
public:
    explicit ExprStmt(std::unique_ptr<Expr> &&);
    std::unique_ptr<Expr> const &get_expr();

    void *accept(IVisitor &visitor, void *ctx) override;

private:
    std::unique_ptr<Expr> _expr;
};

} // namespace slang

#endif //SLANG_EXPRSTMT_H
