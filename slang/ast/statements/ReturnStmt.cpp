#include "ReturnStmt.h"

namespace slang
{
void *ReturnStmt::accept(IVisitor &visitor, void *ctx)
{
    return visitor.visit(*this, ctx);
}
ReturnStmt::ReturnStmt(std::unique_ptr<Expr> &&expr)
    : _expr{std::move(expr)}
{}
std::unique_ptr<Expr> const &ReturnStmt::get_expr() { return _expr; }
} // namespace slang