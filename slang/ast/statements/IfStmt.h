#ifndef SLANG_IFSTMT_H
#define SLANG_IFSTMT_H

#include "expressions/Expr.h"
#include "statements/StmtList.h"
#include <memory>

namespace slang
{

class IfStmt : public Stmt
{
public:
    IfStmt(
        std::unique_ptr<Expr> &&, std::unique_ptr<StmtList> &&,
        std::unique_ptr<StmtList> &&);
    void *accept(IVisitor &visitor, void *ctx) override;

    std::unique_ptr<Expr> const &get_expr();
    std::unique_ptr<StmtList> const &get_stmts();
    std::unique_ptr<StmtList> const &get_else_stmts();

private:
    std::unique_ptr<Expr> _expr;
    std::unique_ptr<StmtList> _stmts;
    std::unique_ptr<StmtList> _else_stmts;
};

} // namespace slang

#endif //SLANG_IFSTMT_H
