
#ifndef SLANG_ASTBUILDER_H
#define SLANG_ASTBUILDER_H

#include "Exception.h"
#include "Program.h"
#include "Token.h"
#include <memory>
#include <vector>

namespace slang
{

class ASTParseException : public Exception
{
public:
    ASTParseException(Token const &, Token::Kind);
    ASTParseException(Token const &, std::string_view);

    [[nodiscard]] const char *what() const noexcept override;

private:
    std::string _msg;
};

class ASTBuilder
{
public:
    explicit ASTBuilder(std::vector<Token> const &);
    ~ASTBuilder();

    std::unique_ptr<Program> build();

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace slang

#endif //SLANG_ASTBUILDER_H
