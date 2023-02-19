#ifndef SLANG_WHILESTMT_H
#define SLANG_WHILESTMT_H

#include "StmtBlock.h"
#include "expressions/Expr.h"
#include <memory>

namespace slang
{

class StmtBlock;

class WhileStmt : public Stmt
{
public:
    [[nodiscard]] std::unique_ptr<Expr> &get_expr();
    [[nodiscard]] std::unique_ptr<StmtBlock> &get_stmts();
    WhileStmt(std::unique_ptr<Expr> &&expr, std::unique_ptr<StmtBlock> &&stmts);
    void *accept(IVisitor &visitor, void *p_void) override;

private:
    std::unique_ptr<Expr> _expr;
    std::unique_ptr<StmtBlock> _stmts;
};

} // namespace slang

#endif //SLANG_WHILESTMT_H
