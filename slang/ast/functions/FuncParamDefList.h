
#ifndef SLANG_FUNCPARAMDEFLIST_H
#define SLANG_FUNCPARAMDEFLIST_H

#include "ASTNode.h"
#include "FuncParamDef.h"
#include "IVisitor.h"

namespace slang
{

class FuncParamDefList : public ASTNode
{
public:
    FuncParamDefList(
        std::unique_ptr<FuncParamDef> &&, std::unique_ptr<FuncParamDefList> &&);

    std::unique_ptr<FuncParamDefList> const &get_next();
    std::unique_ptr<FuncParamDef> const &get_param();

    virtual void *accept(IVisitor &v, void *ctx) { return v.visit(*this, ctx); }

private:
    std::unique_ptr<FuncParamDef> _param;
    std::unique_ptr<FuncParamDefList> _next;
};

} // namespace slang

#endif //SLANG_FUNCPARAMDEFLIST_H
