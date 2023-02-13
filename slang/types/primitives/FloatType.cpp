#include "FloatType.h"
#include "Token.h"

namespace slang
{
std::string_view FloatType::get_name() const { return "float"; }
bool FloatType::is_primitive() const { return true; }
bool FloatType::is_builtin() const { return true; }
bool FloatType::is_assignable_from(Type const *rhs) const { return this == rhs; }
bool FloatType::is_binary_op_compatible(Type const *rhs, Token const &op) const
{
    if (this != rhs)
        return false;
    auto kind = op.get_kind();
    return kind == Token::PLUS || kind == Token::MINUS || kind == Token::DIV ||
           kind == Token::MULT;
}
bool FloatType::is_unary_op_compatible(Token const &op) const { return false; }
} // namespace slang