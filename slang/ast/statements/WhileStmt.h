#ifndef SLANG_WHILESTMT_H
#define SLANG_WHILESTMT_H

#include "expressions/Expr.h"
#include "statements/StmtList.h"
#include <memory>

namespace slang
{

class WhileStmt : public Stmt
{
public:
    [[nodiscard]] std::unique_ptr<Expr> &get_expr();
    [[nodiscard]] std::unique_ptr<StmtList> &get_stmts();
    WhileStmt(std::unique_ptr<Expr> &&expr, std::unique_ptr<StmtList> &&stmts);
    void *accept(IVisitor &visitor, void *p_void) override;

private:
    std::unique_ptr<Expr> _expr;
    std::unique_ptr<StmtList> _stmts;
};

} // namespace slang

#endif //SLANG_WHILESTMT_H
