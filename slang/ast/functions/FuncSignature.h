
#ifndef SLANG_FUNCSIGNATURE_H
#define SLANG_FUNCSIGNATURE_H

#include "ASTNode.h"
#include "FuncParamDefList.h"
#include "FuncRef.h"
#include "TypeRef.h"
#include "visitors/IVisitor.h"

namespace slang
{

class FuncSignature : public ASTNode
{
public:
    FuncSignature(
        std::unique_ptr<TypeRef> &&, std::unique_ptr<FuncRef> &&,
        std::unique_ptr<FuncParamDefList> &&);

    std::unique_ptr<TypeRef> const &get_return_type() const;
    [[nodiscard]] std::unique_ptr<FuncRef> const &get_name() const;
    std::unique_ptr<FuncParamDefList> const &get_params() const;
    void *accept(IVisitor &visitor, void *pVoid) override;

private:
    std::unique_ptr<TypeRef> _return_type;
    std::unique_ptr<FuncRef> _name;
    std::unique_ptr<FuncParamDefList> _params;
};

} // namespace slang

#endif //SLANG_FUNCSIGNATURE_H
