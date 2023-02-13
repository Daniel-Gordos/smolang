#ifndef SLANG_FUNCREF_H
#define SLANG_FUNCREF_H

#include "Symbol.h"
namespace slang
{

class FuncRef : public Symbol
{
public:
    explicit FuncRef(Token);
    void *accept(IVisitor &visitor, void *ctx) override;
    [[nodiscard]] const Token &get_token() const override;

private:
    Token _token;
};

} // namespace slang

#endif //SLANG_FUNCREF_H
