#ifndef SLANG_UNARYEXPR_H
#define SLANG_UNARYEXPR_H

#include "Expr.h"
#include "Token.h"

namespace slang
{

class UnaryExpr : public Expr
{
public:
    UnaryExpr(std::unique_ptr<Expr> &&, Token);
    [[nodiscard]] std::unique_ptr<Expr> const &get_expr() const;
    [[nodiscard]] Token const &get_op() const;

private:
    void *accept(IVisitor &visitor, void *ctx) override;

private:
    std::unique_ptr<Expr> _expr;
    Token _op;
};

} // namespace slang

#endif //SLANG_UNARYEXPR_H
