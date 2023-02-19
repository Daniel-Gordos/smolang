#ifndef SLANG_STMTBLOCK_H
#define SLANG_STMTBLOCK_H

#include "ASTNode.h"
#include "Stmt.h"
#include <memory>
#include <vector>

namespace slang
{

class StmtBlock : public ASTNode
{
public:
    explicit StmtBlock(std::vector<std::unique_ptr<Stmt>>);

    std::vector<std::unique_ptr<Stmt>>::iterator begin();
    std::vector<std::unique_ptr<Stmt>>::iterator end();

    void add_stmt(std::unique_ptr<Stmt>);

    void *accept(IVisitor &visitor, void *p_void) override;

private:
    std::vector<std::unique_ptr<Stmt>> _stmts;
};

} // namespace slang

#endif //SLANG_STMTBLOCK_H
