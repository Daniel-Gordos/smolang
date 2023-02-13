#include "FuncRef.h"

namespace slang
{
FuncRef::FuncRef(Token token)
    : _token{token}
{}
void *FuncRef::accept(IVisitor &visitor, void *ctx) { return visitor.visit(*this, ctx); }
const Token &FuncRef::get_token() const { return _token; }
} // namespace slang