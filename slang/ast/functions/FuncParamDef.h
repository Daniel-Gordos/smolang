
#ifndef SLANG_FUNCPARAMDEF_H
#define SLANG_FUNCPARAMDEF_H

#include "TypeRef.h"
#include "VarRef.h"

namespace slang
{

class FuncParamDef : public ASTNode
{
public:
    FuncParamDef(std::unique_ptr<TypeRef> &&, std::unique_ptr<VarRef> &&);
    std::unique_ptr<TypeRef> const &get_type();
    std::unique_ptr<VarRef> const &get_var();

    virtual void *accept(IVisitor &v, void *ctx) { return v.visit(*this, ctx); }

private:
    std::unique_ptr<TypeRef> _type;
    std::unique_ptr<VarRef> _var;
};

} // namespace slang

#endif //SLANG_FUNCPARAMDEF_H
