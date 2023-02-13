#ifndef SLANG_CALLARGLIST_H
#define SLANG_CALLARGLIST_H

#include "ASTNode.h"
#include "Expr.h"

namespace slang
{

class CallArgList : public ASTNode
{
public:
    CallArgList(std::unique_ptr<Expr> &&, std::unique_ptr<CallArgList> &&);
    std::unique_ptr<Expr> const &get_arg();
    std::unique_ptr<CallArgList> const &get_next();
    void *accept(IVisitor &visitor, void *pVoid) override;

private:
    std::unique_ptr<Expr> _arg;
    std::unique_ptr<CallArgList> _next;
};

} // namespace slang

#endif //SLANG_CALLARGLIST_H
