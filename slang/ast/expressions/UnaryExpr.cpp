#include "UnaryExpr.h"

namespace slang
{
Token const &UnaryExpr::get_op() const { return _op; }
std::unique_ptr<Expr> const &UnaryExpr::get_expr() const { return _expr; }
UnaryExpr::UnaryExpr(std::unique_ptr<Expr> &&expr, Token op)
    : _expr{std::move(expr)}
    , _op{std::move(op)}
{}
void *UnaryExpr::accept(IVisitor &visitor, void *ctx)
{
    return visitor.visit(*this, ctx);
}
} // namespace slang