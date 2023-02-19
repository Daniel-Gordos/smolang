
#ifndef SLANG_FUNCSIGNATURE_H
#define SLANG_FUNCSIGNATURE_H

#include "ASTNode.h"
#include "FuncParamDef.h"
#include "FuncRef.h"
#include "TypeRef.h"
#include "visitors/IVisitor.h"
#include <vector>

namespace slang
{

class FuncSignature : public ASTNode
{
public:
    FuncSignature(
        std::unique_ptr<TypeRef> &&, std::unique_ptr<FuncRef> &&,
        std::vector<std::unique_ptr<FuncParamDef>>);

    std::vector<std::unique_ptr<FuncParamDef>>::iterator begin();
    std::vector<std::unique_ptr<FuncParamDef>>::iterator end();
    std::size_t length() const;

    [[nodiscard]] std::unique_ptr<TypeRef> const &get_return_type() const;
    [[nodiscard]] std::unique_ptr<FuncRef> const &get_name() const;
    void *accept(IVisitor &visitor, void *pVoid) override;

private:
    std::unique_ptr<TypeRef> _return_type;
    std::unique_ptr<FuncRef> _name;
    std::vector<std::unique_ptr<FuncParamDef>> _params;
};

} // namespace slang

#endif //SLANG_FUNCSIGNATURE_H
