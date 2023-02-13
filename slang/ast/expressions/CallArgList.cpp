#include "CallArgList.h"

namespace slang
{
void *CallArgList::accept(IVisitor &visitor, void *ctx)
{
    return visitor.visit(*this, ctx);
}
CallArgList::CallArgList(std::unique_ptr<Expr> &&arg, std::unique_ptr<CallArgList> &&next)
    : _arg{std::move(arg)}
    , _next{std::move(next)}
{}
std::unique_ptr<Expr> const &CallArgList::get_arg() { return _arg; }
std::unique_ptr<CallArgList> const &CallArgList::get_next() { return _next; }
} // namespace slang