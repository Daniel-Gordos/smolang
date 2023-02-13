
#ifndef SLANG_STMTLIST_H
#define SLANG_STMTLIST_H

#include "ASTNode.h"
#include "Stmt.h"
#include "visitors/IVisitor.h"
#include <memory>

namespace slang
{

class StmtList : public ASTNode
{
public:
    StmtList(std::unique_ptr<Stmt> &&, std::unique_ptr<StmtList> &&);
    std::unique_ptr<Stmt> const &get_statement();
    std::unique_ptr<StmtList> const &get_next();

    void *accept(IVisitor &, void *) override;

public:
private:
    std::unique_ptr<Stmt> _stmt;
    std::unique_ptr<StmtList> _next;
};

} // namespace slang

#endif //SLANG_STMTLIST_H
