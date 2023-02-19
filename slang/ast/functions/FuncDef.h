
#ifndef SLANG_FUNCDEF_H
#define SLANG_FUNCDEF_H

#include "ASTNode.h"
#include "FuncSignature.h"
#include "statements/StmtBlock.h"
#include <memory>

namespace slang
{

class StmtBlock;

class FuncDef : public ASTNode
{
public:
    FuncDef(std::unique_ptr<FuncSignature> &&, std::unique_ptr<StmtBlock> &&);
    std::unique_ptr<FuncSignature> const &get_signature();
    std::unique_ptr<StmtBlock> const &get_body();

    void *accept(IVisitor &, void *) override;

private:
    std::unique_ptr<FuncSignature> _signature;
    std::unique_ptr<StmtBlock> _body;
};

} // namespace slang

#endif //SLANG_FUNCDEF_H
