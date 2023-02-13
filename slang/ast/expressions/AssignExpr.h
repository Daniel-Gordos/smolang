#ifndef SLANG_ASSIGNEXPR_H
#define SLANG_ASSIGNEXPR_H
#include "Expr.h"
namespace slang
{

class AssignExpr : public Expr
{
public:
    AssignExpr(std::unique_ptr<Expr> &&lhs, std::unique_ptr<Expr> &&rhs);
    [[nodiscard]] std::unique_ptr<Expr> const &get_lhs();
    [[nodiscard]] std::unique_ptr<Expr> const &get_rhs();
    void *accept(IVisitor &visitor, void *ctx) override;

public:
private:
    std::unique_ptr<Expr> _lhs;
    std::unique_ptr<Expr> _rhs;
};

} // namespace slang

#endif //SLANG_ASSIGNEXPR_H
