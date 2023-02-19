#include "StmtBlock.h"

namespace slang
{
void *StmtBlock::accept(IVisitor &visitor, void *p_void)
{
    return visitor.visit(*this, p_void);
}
StmtBlock::StmtBlock(std::vector<std::unique_ptr<Stmt>> stmts)
    : _stmts(std::move(stmts))
{}
std::vector<std::unique_ptr<Stmt>>::iterator StmtBlock::begin() { return _stmts.begin(); }
std::vector<std::unique_ptr<Stmt>>::iterator StmtBlock::end() { return _stmts.end(); }
void StmtBlock::add_stmt(std::unique_ptr<Stmt> stmt)
{
    _stmts.emplace_back(std::move(stmt));
}
} // namespace slang