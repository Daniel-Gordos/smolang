#include "ExprStmt.h"

namespace slang
{
ExprStmt::ExprStmt(std::unique_ptr<Expr> &&expr)
    : _expr{std::move(expr)}
{}
std::unique_ptr<Expr> const &ExprStmt::get_expr() { return _expr; }
void *ExprStmt::accept(IVisitor &visitor, void *ctx) { return visitor.visit(*this, ctx); }
} // namespace slang