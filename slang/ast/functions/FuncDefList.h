
#ifndef SLANG_FUNCDEFLIST_H
#define SLANG_FUNCDEFLIST_H

#include "ASTNode.h"
#include "FuncDef.h"
#include <memory>

namespace slang
{

class FuncDefList : public ASTNode
{
public:
    explicit FuncDefList(std::unique_ptr<FuncDef> &&);
    FuncDefList(std::unique_ptr<FuncDef> &&, std::unique_ptr<FuncDefList> &&);
    void *accept(IVisitor &visitor, void *pVoid) override;

    std::unique_ptr<FuncDef> const &get_func_def();
    std::unique_ptr<FuncDefList> const &get_next();

private:
    std::unique_ptr<FuncDef> _func;
    std::unique_ptr<FuncDefList> _next;
};

} // namespace slang

#endif //SLANG_FUNCDEFLIST_H
