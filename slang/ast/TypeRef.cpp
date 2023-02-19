
#include "TypeRef.h"

namespace slang
{
TypeRef::TypeRef(Token token)
    : _token(token)
{}
void *TypeRef::accept(IVisitor &visitor, void *ctx) { return visitor.visit(*this, ctx); }
const Token &TypeRef::get_token() const { return _token; }
Type *TypeRef::get_type() { return _type; }
void TypeRef::set_type(Type *t) { _type = t; }
} // namespace slang