
#include "FuncParamDefList.h"

namespace slang
{
FuncParamDefList::FuncParamDefList(
    std::unique_ptr<FuncParamDef> &&param, std::unique_ptr<FuncParamDefList> &&next)
    : _param{std::move(param)}
    , _next(std::move(next))
{}
std::unique_ptr<FuncParamDefList> const &FuncParamDefList::get_next() { return _next; }
std::unique_ptr<FuncParamDef> const &FuncParamDefList::get_param() { return _param; }
} // namespace slang