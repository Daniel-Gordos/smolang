#ifndef SLANG_LITERALEXPR_H
#define SLANG_LITERALEXPR_H

#include "Expr.h"

namespace slang
{

class LiteralExpr : public Expr
{
public:
    explicit LiteralExpr(Token);
    void *accept(IVisitor &visitor, void *ctx) override;
    [[nodiscard]] Token const &get_token() const;

private:
    Token _token;
};

} // namespace slang

#endif //SLANG_LITERALEXPR_H
