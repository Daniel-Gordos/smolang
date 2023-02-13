#ifndef SLANG_VARREFEXPR_H
#define SLANG_VARREFEXPR_H
#include "RefExpr.h"
#include "VarRef.h"
namespace slang
{

class VarRefExpr : public RefExpr
{
public:
    explicit VarRefExpr(std::unique_ptr<VarRef> &&);
    [[nodiscard]] std::unique_ptr<VarRef> const &get_var() const;
    void *accept(IVisitor &visitor, void *ctx) override;

private:
    std::unique_ptr<VarRef> _var;
};

} // namespace slang

#endif //SLANG_VARREFEXPR_H
