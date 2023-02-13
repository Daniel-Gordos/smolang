
#include "FuncDefList.h"

namespace slang
{

FuncDefList::FuncDefList(std::unique_ptr<FuncDef> &&f)
    : _func(std::move(f))
{}

FuncDefList::FuncDefList(
    std::unique_ptr<FuncDef> &&f, std::unique_ptr<FuncDefList> &&next)
    : _func(std::move(f))
    , _next(std::move(next))
{}

std::unique_ptr<FuncDef> const &FuncDefList::get_func_def() { return _func; }
std::unique_ptr<FuncDefList> const &FuncDefList::get_next() { return _next; }
void *FuncDefList::accept(IVisitor &visitor, void *ctx)
{
    return visitor.visit(*this, ctx);
}

} // namespace slang