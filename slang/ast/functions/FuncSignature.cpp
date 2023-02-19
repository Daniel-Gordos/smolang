
#include "FuncSignature.h"

namespace slang
{
FuncSignature::FuncSignature(
    std::unique_ptr<TypeRef> &&return_type, std::unique_ptr<FuncRef> &&name,
    std::vector<std::unique_ptr<FuncParamDef>> params)
    : _return_type{std::move(return_type)}
    , _name{std::move(name)}
    , _params{std::move(params)}
{}
std::unique_ptr<TypeRef> const &FuncSignature::get_return_type() const
{
    return _return_type;
}
std::unique_ptr<FuncRef> const &FuncSignature::get_name() const { return _name; }
void *FuncSignature::accept(IVisitor &visitor, void *ctx)
{
    return visitor.visit(*this, ctx);
}
std::vector<std::unique_ptr<FuncParamDef>>::iterator FuncSignature::begin()
{
    return _params.begin();
}
std::vector<std::unique_ptr<FuncParamDef>>::iterator FuncSignature::end()
{
    return _params.end();
}
std::size_t FuncSignature::length() const { return _params.size(); }
} // namespace slang