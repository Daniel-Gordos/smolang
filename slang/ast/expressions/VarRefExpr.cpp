#include "VarRefExpr.h"

namespace slang
{
VarRefExpr::VarRefExpr(std::unique_ptr<VarRef> &&var)
    : _var{std::move(var)}
{}
std::unique_ptr<VarRef> const &VarRefExpr::get_var() const { return _var; }
void *VarRefExpr::accept(IVisitor &visitor, void *ctx)
{
    return visitor.visit(*this, ctx);
}
} // namespace slang