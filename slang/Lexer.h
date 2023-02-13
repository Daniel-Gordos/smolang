
#ifndef SLANG_LEXER_H
#define SLANG_LEXER_H

#include "SourceFile.h"
#include "Token.h"

#include <memory>
#include <vector>

namespace slang
{

class Lexer
{
public:
    explicit Lexer(SourceFile &);
    ~Lexer();

    [[nodiscard]] std::vector<slang::Token> parse_tokens();

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace slang

#endif //SLANG_LEXER_H
