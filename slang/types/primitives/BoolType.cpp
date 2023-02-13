#include "BoolType.h"

namespace slang
{
bool BoolType::is_assignable_from(Type const *rhs) const { return this == rhs; }

bool BoolType::is_binary_op_compatible(Type const *rhs, Token const &op) const
{
    if (rhs != this)
        return false;

    auto kind = op.get_kind();
    return kind == Token::AND || kind == Token::OR || kind == Token::EQEQ ||
           kind == Token::NOTEQ;
}
bool BoolType::is_unary_op_compatible(Token const &op) const
{
    return op.get_kind() == Token::NOT;
}
std::string_view BoolType::get_name() const { return "bool"; }
bool BoolType::is_primitive() const { return true; }
bool BoolType::is_builtin() const { return true; }

} // namespace slang