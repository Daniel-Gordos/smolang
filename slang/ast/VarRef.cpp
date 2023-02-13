
#include "VarRef.h"

namespace slang
{
VarRef::VarRef(Token token)
    : _token{token}
{}
void *VarRef::accept(IVisitor &visitor, void *ctx) { visitor.visit(*this, ctx); }
const Token &VarRef::get_token() const { return _token; }
} // namespace slang