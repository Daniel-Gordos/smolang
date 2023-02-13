
#ifndef SLANG_FUNCDEF_H
#define SLANG_FUNCDEF_H

#include "ASTNode.h"
#include "FuncBody.h"
#include "FuncSignature.h"
#include <memory>

namespace slang
{

class FuncDef : public ASTNode
{
public:
    FuncDef(std::unique_ptr<FuncSignature> &&, std::unique_ptr<FuncBody> &&);
    std::unique_ptr<FuncSignature> const &get_signature();
    std::unique_ptr<FuncBody> const &get_body();

    void *accept(IVisitor &, void *) override;

private:
    std::unique_ptr<FuncSignature> _signature;
    std::unique_ptr<FuncBody> _body;
};

} // namespace slang

#endif //SLANG_FUNCDEF_H
