#include "WhileStmt.h"

namespace slang
{
WhileStmt::WhileStmt(std::unique_ptr<Expr> &&expr, std::unique_ptr<StmtList> &&stmts)
    : _expr(std::move(expr))
    , _stmts(std::move(stmts))
{}
std::unique_ptr<Expr> &WhileStmt::get_expr() { return _expr; }
std::unique_ptr<StmtList> &WhileStmt::get_stmts() { return _stmts; }

void *WhileStmt::accept(IVisitor &visitor, void *p_void)
{
    return visitor.visit(*this, p_void);
}
} // namespace slang