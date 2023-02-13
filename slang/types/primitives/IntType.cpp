#include "IntType.h"
#include "Token.h"

namespace slang
{
std::string_view IntType::get_name() const { return "int"; }
bool IntType::is_primitive() const { return true; }
bool IntType::is_builtin() const { return true; }
bool IntType::is_assignable_from(Type const *rhs) const { return this == rhs; }
bool IntType::is_binary_op_compatible(Type const *rhs, Token const &op) const
{
    if (this != rhs)
        return false;
    auto kind = op.get_kind();
    auto it = std::find(BINARY_OPS.begin(), BINARY_OPS.end(), op.get_kind());
    return it != BINARY_OPS.end();
}
bool IntType::is_unary_op_compatible(Token const &op) const
{
    auto kind = op.get_kind();
    return kind == Token::PLUS || kind == Token::MINUS;
}
const std::vector<const Token::Kind> IntType::BINARY_OPS = {
    Token::PLUS, Token::MINUS, Token::MULT, Token::DIV,  Token::GT,
    Token::GE,   Token::LT,    Token::LE,   Token::EQEQ, Token::NOTEQ,
};
} // namespace slang