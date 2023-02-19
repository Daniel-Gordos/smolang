
#ifndef SLANG_PROGRAM_H
#define SLANG_PROGRAM_H

#include "ASTNode.h"
#include "functions/FuncDef.h"
#include "visitors/IVisitor.h"
#include <memory>
#include <vector>

namespace slang
{

class Program : public ASTNode
{
public:
    Program() = default;
    void *accept(IVisitor &visitor, void *pVoid) override;

    void add_func(std::unique_ptr<FuncDef>);

    std::vector<std::unique_ptr<FuncDef>>::iterator begin();
    std::vector<std::unique_ptr<FuncDef>>::iterator end();

private:
    std::vector<std::unique_ptr<FuncDef>> _funcs{};
};

} // namespace slang

#endif //SLANG_PROGRAM_H
