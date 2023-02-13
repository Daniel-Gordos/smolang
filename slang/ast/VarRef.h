
#ifndef SLANG_VARREF_H
#define SLANG_VARREF_H

#include "Symbol.h"

namespace slang
{

class VarRef : public Symbol
{
public:
    explicit VarRef(Token);
    const Token &get_token() const override;

public:
    void *accept(IVisitor &visitor, void *pVoid) override;

private:
    Token _token;
};

} // namespace slang

#endif //SLANG_VARREF_H
