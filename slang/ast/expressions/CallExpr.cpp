#include "CallExpr.h"
#include "functions/FuncRef.h"

namespace slang
{
CallExpr::CallExpr(std::unique_ptr<FuncRef> &&name, std::unique_ptr<CallArgList> &&args)
    : _name{std::move(name)}
    , _args{std::move(args)}
{}
void *CallExpr::accept(IVisitor &visitor, void *ctx) { return visitor.visit(*this, ctx); }
std::unique_ptr<FuncRef> const &CallExpr::get_name() { return _name; }
std::unique_ptr<CallArgList> const &CallExpr::get_args() { return _args; }
} // namespace slang