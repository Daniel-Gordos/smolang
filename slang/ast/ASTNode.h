
#ifndef SLANG_ASTNODE_H
#define SLANG_ASTNODE_H

#include "IVisitor.h"
#include "Token.h"

namespace slang
{

class ASTNode
{
public:
    ASTNode() = default;
    explicit ASTNode(TextPosition pos)
        : _pos{pos}
    {}
    virtual ~ASTNode() = default;

    [[nodiscard]] TextPosition &get_pos() { return _pos; }
    void set_pos(TextPosition p) { _pos = p; }

    virtual void *accept(IVisitor &, void *) = 0;

protected:
    TextPosition _pos;
};

} // namespace slang

#endif //SLANG_ASTNODE_H
