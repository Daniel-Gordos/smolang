#include "DerefExpr.h"

namespace slang
{
std::unique_ptr<RefExpr> const &DerefExpr::get_ref() const { return _ref; }
DerefExpr::DerefExpr(std::unique_ptr<RefExpr> &&ref_expr)
    : _ref{std::move(ref_expr)}
{}
void *DerefExpr::accept(IVisitor &visitor, void *ctx)
{
    return visitor.visit(*this, ctx);
}
} // namespace slang