#ifndef SLANG_FLOATTYPE_H
#define SLANG_FLOATTYPE_H

#include "Type.h"

namespace slang
{

class FloatType : public Type
{
public:
    FloatType() = default;
    std::string_view get_name() const override;
    bool is_primitive() const override;
    bool is_builtin() const override;
    bool is_assignable_from(Type const *rhs) const override;
    bool is_binary_op_compatible(Type const *rhs, Token const &op) const override;
    bool is_unary_op_compatible(Token const &op) const override;
};

} // namespace slang

#endif //SLANG_FLOATTYPE_H
