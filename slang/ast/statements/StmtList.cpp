
#include "StmtList.h"

namespace slang
{
void *StmtList::accept(slang::IVisitor &visitor, void *ctx)
{
    return visitor.visit(*this, ctx);
}

StmtList::StmtList(std::unique_ptr<Stmt> &&stmt, std::unique_ptr<StmtList> &&next)
    : _stmt{std::move(stmt)}
    , _next{std::move(next)}
{}
std::unique_ptr<Stmt> const &StmtList::get_statement() { return _stmt; }
std::unique_ptr<StmtList> const &StmtList::get_next() { return _next; }
} // namespace slang
