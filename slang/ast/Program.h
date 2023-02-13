
#ifndef SLANG_PROGRAM_H
#define SLANG_PROGRAM_H

#include "ASTNode.h"
#include "ast/functions/FuncDefList.h"
#include "visitors/IVisitor.h"
#include <memory>

namespace slang
{

class Program : public ASTNode
{
public:
    explicit Program(std::unique_ptr<FuncDefList> &&);
    void *accept(IVisitor &visitor, void *pVoid) override;
    std::unique_ptr<FuncDefList> func_def_list;
};

} // namespace slang

#endif //SLANG_PROGRAM_H
