#include "BinaryExpr.h"

namespace slang
{
BinaryExpr::BinaryExpr(std::unique_ptr<Expr> &&lhs, std::unique_ptr<Expr> &&rhs, Token op)
    : _lhs{std::move(lhs)}
    , _rhs{std::move(rhs)}
    , _op{std::move(op)}
{}
void *BinaryExpr::accept(IVisitor &visitor, void *ctx)
{
    return visitor.visit(*this, ctx);
}
std::unique_ptr<Expr> const &BinaryExpr::get_lhs() const { return _lhs; }
std::unique_ptr<Expr> const &BinaryExpr::get_rhs() const { return _rhs; }
Token const &BinaryExpr::get_op() const { return _op; }
} // namespace slang