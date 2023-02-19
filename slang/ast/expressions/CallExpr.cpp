#include "CallExpr.h"
#include "functions/FuncRef.h"

namespace slang
{
CallExpr::CallExpr(
    std::unique_ptr<FuncRef> &&func, std::vector<std::unique_ptr<Expr>> args)
    : _name(std::move(func))
    , _args(std::move(args))
{}
void *CallExpr::accept(IVisitor &visitor, void *ctx) { return visitor.visit(*this, ctx); }
std::unique_ptr<FuncRef> const &CallExpr::get_name() { return _name; }
std::vector<std::unique_ptr<Expr>>::iterator CallExpr::begin() { return _args.begin(); }
std::vector<std::unique_ptr<Expr>>::iterator CallExpr::end() { return _args.end(); }
std::size_t CallExpr::length() const { return _args.size(); }
} // namespace slang