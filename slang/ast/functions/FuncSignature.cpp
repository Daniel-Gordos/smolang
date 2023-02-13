
#include "FuncSignature.h"

namespace slang
{
FuncSignature::FuncSignature(
    std::unique_ptr<TypeRef> &&return_type, std::unique_ptr<FuncRef> &&name,
    std::unique_ptr<FuncParamDefList> &&params)
    : _return_type{std::move(return_type)}
    , _name{std::move(name)}
    , _params{std::move(params)}
{}
std::unique_ptr<TypeRef> const &FuncSignature::get_return_type() const { return _return_type; }
std::unique_ptr<FuncRef> const &FuncSignature::get_name() const { return _name; }
std::unique_ptr<FuncParamDefList> const &FuncSignature::get_params() const { return _params; }
void * FuncSignature::accept(IVisitor &visitor, void *ctx) { visitor.visit(*this, ctx); }
} // namespace slang