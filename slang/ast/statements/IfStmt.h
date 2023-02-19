#ifndef SLANG_IFSTMT_H
#define SLANG_IFSTMT_H

#include "StmtBlock.h"
#include "expressions/Expr.h"
#include <memory>

namespace slang
{

class StmtBlock;

class IfStmt : public Stmt
{
public:
    IfStmt(
        std::unique_ptr<Expr> &&, std::unique_ptr<StmtBlock> &&,
        std::unique_ptr<StmtBlock> &&);
    void *accept(IVisitor &visitor, void *ctx) override;

    std::unique_ptr<Expr> const &get_expr();
    std::unique_ptr<StmtBlock> const &get_stmts();
    std::unique_ptr<StmtBlock> const &get_else_stmts();

private:
    std::unique_ptr<Expr> _expr;
    std::unique_ptr<StmtBlock> _stmts;
    std::unique_ptr<StmtBlock> _else_stmts;
};

} // namespace slang

#endif //SLANG_IFSTMT_H
