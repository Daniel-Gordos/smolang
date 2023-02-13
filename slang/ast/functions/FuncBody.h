#ifndef SLANG_FUNCBODY_H
#define SLANG_FUNCBODY_H
#include "ASTNode.h"
#include "statements/StmtList.h"
namespace slang
{

class FuncBody : public ASTNode
{
public:
    explicit FuncBody(std::unique_ptr<StmtList> &&);
    [[nodiscard]] std::unique_ptr<StmtList> const &get_statements() const;
    void *accept(IVisitor &visitor, void *ctx) override;

private:
    std::unique_ptr<StmtList> _stmts;
};

} // namespace slang

#endif //SLANG_FUNCBODY_H
