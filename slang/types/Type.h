#ifndef SLANG_TYPE_H
#define SLANG_TYPE_H

#include <string>

namespace slang
{

class Token;

class Type
{
public:
    [[nodiscard]] virtual std::string_view get_name() const = 0;
    [[nodiscard]] virtual bool is_primitive() const = 0;
    [[nodiscard]] virtual bool is_builtin() const = 0;
    [[nodiscard]] virtual bool is_assignable_from(const Type *rhs) const = 0;
    [[nodiscard]] virtual bool
    is_binary_op_compatible(const Type *rhs, const Token &op) const = 0;
    [[nodiscard]] virtual bool is_unary_op_compatible(const Token &op) const = 0;

    virtual ~Type() = default;
};

} // namespace slang

#endif //SLANG_TYPE_H
