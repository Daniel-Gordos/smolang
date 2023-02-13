#ifndef SLANG_BINARYEXPR_H
#define SLANG_BINARYEXPR_H
#include "Expr.h"
#include "Token.h"

namespace slang
{

class BinaryExpr : public Expr
{
public:
    BinaryExpr(std::unique_ptr<Expr> &&, std::unique_ptr<Expr> &&, Token);
    void *accept(IVisitor &visitor, void *pVoid) override;
    [[nodiscard]] std::unique_ptr<Expr> const &get_lhs() const;
    [[nodiscard]] std::unique_ptr<Expr> const &get_rhs() const;
    Token const &get_op() const;

private:
    std::unique_ptr<Expr> _lhs;
    std::unique_ptr<Expr> _rhs;
    Token _op;
};

} // namespace slang

#endif //SLANG_BINARYEXPR_H
