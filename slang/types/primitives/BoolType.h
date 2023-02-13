#ifndef SLANG_BOOLTYPE_H
#define SLANG_BOOLTYPE_H

#include "Token.h"
#include "Type.h"
#include <vector>
namespace slang
{

class BoolType : public Type
{
public:
    BoolType() = default;
    bool is_assignable_from(Type const *rhs) const override;
    bool is_binary_op_compatible(Type const *rhs, Token const &op) const override;
    bool is_unary_op_compatible(Token const &op) const override;
    std::string_view get_name() const override;
    bool is_primitive() const override;
    bool is_builtin() const override;
};

} // namespace slang

#endif //SLANG_BOOLTYPE_H
