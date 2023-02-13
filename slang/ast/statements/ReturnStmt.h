#ifndef SLANG_RETURNSTMT_H
#define SLANG_RETURNSTMT_H
#include "expressions/Expr.h"
#include "statements/Stmt.h"
namespace slang
{

class ReturnStmt : public Stmt
{
public:
    explicit ReturnStmt(std::unique_ptr<Expr> &&);
    std::unique_ptr<Expr> const &get_expr();
    void *accept(IVisitor &visitor, void *ctx) override;

private:
    std::unique_ptr<Expr> _expr;
};

} // namespace slang

#endif //SLANG_RETURNSTMT_H
