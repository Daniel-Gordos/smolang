#ifndef SLANG_DEREFEXPR_H
#define SLANG_DEREFEXPR_H
#include "Expr.h"
#include "RefExpr.h"
namespace slang
{

class DerefExpr : public Expr
{
public:
    explicit DerefExpr(std::unique_ptr<RefExpr> &&);
    [[nodiscard]] std::unique_ptr<RefExpr> const &get_ref() const;
    void *accept(IVisitor &visitor, void *ctx) override;

private:
    std::unique_ptr<RefExpr> _ref;
};

} // namespace slang

#endif //SLANG_DEREFEXPR_H
