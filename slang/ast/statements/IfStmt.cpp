#include "IfStmt.h"

namespace slang
{
void *IfStmt::accept(IVisitor &visitor, void *ctx) { return visitor.visit(*this, ctx); }
IfStmt::IfStmt(
    std::unique_ptr<Expr> &&expr, std::unique_ptr<StmtBlock> &&stmts,
    std::unique_ptr<StmtBlock> &&else_stmts)
    : _expr{std::move(expr)}
    , _stmts{std::move(stmts)}
    , _else_stmts{std::move(else_stmts)}
{}
std::unique_ptr<Expr> const &IfStmt::get_expr() { return _expr; }
std::unique_ptr<StmtBlock> const &IfStmt::get_stmts() { return _stmts; }
std::unique_ptr<StmtBlock> const &IfStmt::get_else_stmts() { return _else_stmts; }
} // namespace slang