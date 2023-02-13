#include "InferredDeclExpr.h"

namespace slang
{
std::unique_ptr<VarRefExpr> const &InferredDeclExpr::get_lhs() { return _lhs; }
std::unique_ptr<Expr> const &InferredDeclExpr::get_rhs() { return _rhs; }
InferredDeclExpr::InferredDeclExpr(
    std::unique_ptr<VarRefExpr> &&lhs, std::unique_ptr<Expr> &&rhs)
    : _lhs{std::move(lhs)}
    , _rhs{std::move(rhs)}
{}
void *InferredDeclExpr::accept(IVisitor &visitor, void *ctx)
{
    return visitor.visit(*this, ctx);
}
} // namespace slang