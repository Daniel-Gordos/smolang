#ifndef SLANG_CALLEXPR_H
#define SLANG_CALLEXPR_H

#include "Expr.h"
#include "VarRef.h"
#include <vector>

namespace slang
{
class CallExpr : public Expr
{
public:
    CallExpr(std::unique_ptr<FuncRef> &&, std::vector<std::unique_ptr<Expr>>);
    std::unique_ptr<FuncRef> const &get_name();

    std::vector<std::unique_ptr<Expr>>::iterator begin();
    std::vector<std::unique_ptr<Expr>>::iterator end();
    std::size_t length() const;

    void *accept(IVisitor &visitor, void *ctx) override;

private:
    std::unique_ptr<FuncRef> _name;
    std::vector<std::unique_ptr<Expr>> _args;
};

} // namespace slang

#endif //SLANG_CALLEXPR_H
