
#ifndef SLANG_TYPEREF_H
#define SLANG_TYPEREF_H

#include "Symbol.h"

namespace slang
{

class Type;

class TypeRef : public Symbol
{
public:
    [[nodiscard]] const Token &get_token() const override;
    [[nodiscard]] Type *get_type();
    void set_type(Type *);

public:
    explicit TypeRef(Token);
    void *accept(IVisitor &visitor, void *ctx) override;

private:
    Token _token;
    Type *_type = nullptr;
};

} // namespace slang

#endif //SLANG_TYPEREF_H
