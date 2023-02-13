#include "FuncBody.h"

namespace slang
{
FuncBody::FuncBody(std::unique_ptr<StmtList> &&stmts)
    : _stmts{std::move(stmts)}
{}
void *FuncBody::accept(IVisitor &visitor, void *ctx) {return visitor.visit(*this, ctx); }
std::unique_ptr<StmtList> const &FuncBody::get_statements() const { return _stmts; }
} // namespace slang