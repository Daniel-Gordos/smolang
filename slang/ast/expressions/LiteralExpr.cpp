#include "LiteralExpr.h"

namespace slang
{
Token const &LiteralExpr::get_token() const { return _token; }
LiteralExpr::LiteralExpr(Token token)
    : _token{std::move(token)}
{}
void *LiteralExpr::accept(IVisitor &visitor, void *ctx) {return visitor.visit(*this, ctx); }
} // namespace slang