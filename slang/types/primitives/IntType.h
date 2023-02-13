#ifndef SLANG_INTTYPE_H
#define SLANG_INTTYPE_H

#include "Token.h"
#include "Type.h"
#include <vector>

namespace slang
{

class IntType : public Type
{
public:
    IntType() = default;
    std::string_view get_name() const override;
    bool is_primitive() const override;
    bool is_builtin() const override;
    bool is_assignable_from(Type const *rhs) const override;
    bool is_binary_op_compatible(Type const *rhs, Token const &op) const override;
    bool is_unary_op_compatible(Token const &op) const override;

private:
    static const std::vector<const Token::Kind> BINARY_OPS;
};

} // namespace slang

#endif //SLANG_INTTYPE_H
