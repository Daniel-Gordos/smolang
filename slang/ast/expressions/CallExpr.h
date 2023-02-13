#ifndef SLANG_CALLEXPR_H
#define SLANG_CALLEXPR_H

#include "Expr.h"
#include "VarRef.h"
#include "expressions/CallArgList.h"

namespace slang
{
class CallExpr : public Expr
{
public:
    CallExpr(std::unique_ptr<FuncRef> &&, std::unique_ptr<CallArgList> &&);
    std::unique_ptr<FuncRef> const &get_name();
    std::unique_ptr<CallArgList> const &get_args();

    void *accept(IVisitor &visitor, void *ctx) override;

private:
    std::unique_ptr<FuncRef> _name;
    std::unique_ptr<CallArgList> _args;
};

} // namespace slang

#endif //SLANG_CALLEXPR_H
