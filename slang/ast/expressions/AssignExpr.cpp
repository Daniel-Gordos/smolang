#include "AssignExpr.h"

namespace slang
{
AssignExpr::AssignExpr(std::unique_ptr<Expr> &&lhs, std::unique_ptr<Expr> &&rhs)
    : _lhs(std::move(lhs))
    , _rhs(std::move(rhs))
{}
std::unique_ptr<Expr> const &AssignExpr::get_lhs() { return _lhs; }
std::unique_ptr<Expr> const &AssignExpr::get_rhs() { return _rhs; }
void *AssignExpr::accept(IVisitor &visitor, void *ctx) {return visitor.visit(*this, ctx); }
} // namespace slang