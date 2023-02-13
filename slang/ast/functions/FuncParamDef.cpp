
#include "FuncParamDef.h"

namespace slang
{
FuncParamDef::FuncParamDef(std::unique_ptr<TypeRef> &&type, std::unique_ptr<VarRef> &&var)
    : _type{std::move(type)}
    , _var{std::move(var)}
{}
std::unique_ptr<TypeRef> const &FuncParamDef::get_type() { return _type; }
std::unique_ptr<VarRef> const &FuncParamDef::get_var() { return _var; }
} // namespace slang