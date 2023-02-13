#ifndef SLANG_SYMBOL_H
#define SLANG_SYMBOL_H

#include "ASTNode.h"

namespace slang
{

class Symbol : public ASTNode
{
public:
    [[nodiscard]] virtual Token const &get_token() const = 0;
};

} // namespace slang

#endif //SLANG_SYMBOL_H
