
#include "FuncDef.h"

namespace slang
{
FuncDef::FuncDef(std::unique_ptr<FuncSignature> &&sig, std::unique_ptr<FuncBody> &&body)
    : _signature{std::move(sig)}
    , _body{std::move(body)}
{}
std::unique_ptr<FuncSignature> const &FuncDef::get_signature() { return _signature; }
std::unique_ptr<FuncBody> const &FuncDef::get_body() { return _body; }
void *FuncDef::accept(IVisitor &visitor, void *ctx) { return visitor.visit(*this, ctx); }
} // namespace slang