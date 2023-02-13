#ifndef SLANG_INFERREDDECLEXPR_H
#define SLANG_INFERREDDECLEXPR_H

#include "Expr.h"
#include "VarRefExpr.h"
#include <memory>

namespace slang
{

class InferredDeclExpr : public Expr
{
public:
    InferredDeclExpr(std::unique_ptr<VarRefExpr> &&, std::unique_ptr<Expr> &&);
    std::unique_ptr<VarRefExpr> const &get_lhs();
    std::unique_ptr<Expr> const &get_rhs();
    void *accept(IVisitor &visitor, void *ctx) override;

private:
    std::unique_ptr<VarRefExpr> _lhs;
    std::unique_ptr<Expr> _rhs;
};

} // namespace slang

#endif //SLANG_INFERREDDECLEXPR_H
