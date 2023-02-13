#ifndef SLANG_EXPR_H
#define SLANG_EXPR_H

#include "ASTNode.h"

namespace slang
{
class Type;
class Expr : public ASTNode
{
public:
    [[nodiscard]] Type *get_type() const { return _type_ptr; }
    void set_type(Type *t) { _type_ptr = t; }

private:
    Type *_type_ptr;
};

} // namespace slang

#endif //SLANG_EXPR_H
