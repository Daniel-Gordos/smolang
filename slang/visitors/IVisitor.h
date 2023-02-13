#ifndef SLANG_IVISITOR_H
#define SLANG_IVISITOR_H

namespace slang
{

// Forward declare to avoid circular deps
class Program;

class FuncDefList;
class FuncDef;
class FuncSignature;
class FuncParamDefList;
class FuncParamDef;
class FuncBody;

class TypeRef;
class FuncRef;
class VarRef;

class StmtList;
class Stmt;
class ExprStmt;
class ReturnStmt;

class IfStmt;
class WhileStmt;

class InferredDeclExpr;
class BinaryExpr;
class UnaryExpr;
class VarRefExpr;
class LiteralExpr;
class DerefExpr;
class AssignExpr;

class CallExpr;
class CallArgList;

class ASTNode;

class IVisitor
{
public:
    virtual ~IVisitor() = default;

    virtual void *visit(Program &, void *) = 0;
    virtual void *visit(FuncDef &, void *) = 0;
    virtual void *visit(FuncDefList &, void *) = 0;
    virtual void *visit(FuncSignature &, void *) = 0;
    virtual void *visit(StmtList &, void *) = 0;
    virtual void *visit(FuncParamDefList &, void *) = 0;
    virtual void *visit(FuncParamDef &, void *) = 0;
    virtual void *visit(FuncBody &, void *) = 0;
    virtual void *visit(TypeRef &, void *) = 0;
    virtual void *visit(FuncRef &, void *) = 0;
    virtual void *visit(VarRef &, void *) = 0;
    virtual void *visit(ExprStmt &, void *) = 0;
    virtual void *visit(InferredDeclExpr &, void *) = 0;
    virtual void *visit(BinaryExpr &, void *) = 0;
    virtual void *visit(UnaryExpr &, void *) = 0;
    virtual void *visit(VarRefExpr &, void *) = 0;
    virtual void *visit(LiteralExpr &, void *) = 0;
    virtual void *visit(DerefExpr &, void *) = 0;
    virtual void *visit(AssignExpr &, void *) = 0;
    virtual void *visit(IfStmt &, void *) = 0;
    virtual void *visit(WhileStmt &, void *) = 0;
    virtual void *visit(ReturnStmt &, void *) = 0;
    virtual void *visit(CallExpr &, void *) = 0;
    virtual void *visit(CallArgList &, void *) = 0;
};

// Visitor type with strongly typed context parameter and return parameter
// Visitor funcs are no-op unless overridden.
template<typename C, typename R>
class AbstractVisitor : public IVisitor
{

protected:
    AbstractVisitor() = default;

public:
    virtual R *visit_type(Program &, C *) { return nullptr; }
    virtual R *visit_type(FuncDef &, C *) { return nullptr; }
    virtual R *visit_type(FuncDefList &, C *) { return nullptr; }
    virtual R *visit_type(FuncSignature &, C *) { return nullptr; }
    virtual R *visit_type(FuncParamDef &, C *) { return nullptr; }
    virtual R *visit_type(FuncParamDefList &, C *) { return nullptr; }
    virtual R *visit_type(FuncBody &, C *) { return nullptr; }
    virtual R *visit_type(StmtList &, C *) { return nullptr; }
    virtual R *visit_type(TypeRef &, C *) { return nullptr; }
    virtual R *visit_type(FuncRef &, C *) { return nullptr; }
    virtual R *visit_type(VarRef &, C *) { return nullptr; }
    virtual R *visit_type(InferredDeclExpr &, C *) { return nullptr; }
    virtual R *visit_type(ExprStmt &, C *) { return nullptr; }
    virtual R *visit_type(BinaryExpr &, C *) { return nullptr; }
    virtual R *visit_type(UnaryExpr &, C *) { return nullptr; }
    virtual R *visit_type(VarRefExpr &, C *) { return nullptr; }
    virtual R *visit_type(LiteralExpr &, C *) { return nullptr; }
    virtual R *visit_type(DerefExpr &, C *) { return nullptr; }
    virtual R *visit_type(AssignExpr &, C *) { return nullptr; }
    virtual R *visit_type(IfStmt &, C *) { return nullptr; }
    virtual R *visit_type(WhileStmt &, C *) { return nullptr; }
    virtual R *visit_type(ReturnStmt &, C *) { return nullptr; }
    virtual R *visit_type(CallExpr &, C *) { return nullptr; }
    virtual R *visit_type(CallArgList &, C *) { return nullptr; }

    void *visit(slang::Program &p, void *ctx) final
    {
        return visit_type(p, static_cast<C *>(ctx));
    }
    void *visit(slang::FuncDefList &p, void *ctx) final
    {
        return visit_type(p, static_cast<C *>(ctx));
    }
    void *visit(slang::FuncDef &p, void *ctx) final
    {
        return visit_type(p, static_cast<C *>(ctx));
    }
    void *visit(slang::FuncSignature &p, void *ctx) final
    {
        return visit_type(p, static_cast<C *>(ctx));
    }
    void *visit(slang::FuncParamDefList &p, void *ctx) final
    {
        return visit_type(p, static_cast<C *>(ctx));
    }
    void *visit(slang::FuncParamDef &p, void *ctx) final
    {
        return visit_type(p, static_cast<C *>(ctx));
    }
    void *visit(slang::StmtList &p, void *ctx) final
    {
        return visit_type(p, static_cast<C *>(ctx));
    }
    void *visit(slang::TypeRef &p, void *ctx) final
    {
        return visit_type(p, static_cast<C *>(ctx));
    }
    void *visit(slang::VarRef &p, void *ctx) final
    {
        return visit_type(p, static_cast<C *>(ctx));
    }
    void *visit(slang::ExprStmt &p, void *ctx) final
    {
        return visit_type(p, static_cast<C *>(ctx));
    }
    void *visit(slang::InferredDeclExpr &p, void *ctx) final
    {
        return visit_type(p, static_cast<C *>(ctx));
    }
    void *visit(slang::BinaryExpr &p, void *ctx) final
    {
        return visit_type(p, static_cast<C *>(ctx));
    }
    void *visit(slang::UnaryExpr &p, void *ctx) final
    {
        return visit_type(p, static_cast<C *>(ctx));
    }
    void *visit(slang::DerefExpr &p, void *ctx) final
    {
        return visit_type(p, static_cast<C *>(ctx));
    }
    void *visit(slang::VarRefExpr &p, void *ctx) final
    {
        return visit_type(p, static_cast<C *>(ctx));
    }
    void *visit(slang::LiteralExpr &p, void *ctx) final
    {
        return visit_type(p, static_cast<C *>(ctx));
    }
    void *visit(slang::FuncBody &p, void *ctx) final
    {
        return visit_type(p, static_cast<C *>(ctx));
    }
    void *visit(slang::AssignExpr &p, void *ctx) final
    {
        return visit_type(p, static_cast<C *>(ctx));
    }
    void *visit(slang::IfStmt &p, void *ctx) final
    {
        return visit_type(p, static_cast<C *>(ctx));
    }
    void *visit(slang::ReturnStmt &p, void *ctx) final
    {
        return visit_type(p, static_cast<C *>(ctx));
    }
    void *visit(slang::WhileStmt &p, void *ctx) final
    {
        return visit_type(p, static_cast<C *>(ctx));
    }
    void *visit(slang::CallExpr &p, void *ctx) final
    {
        return visit_type(p, static_cast<C *>(ctx));
    }
    void *visit(slang::CallArgList &p, void *ctx) final
    {
        return visit_type(p, static_cast<C *>(ctx));
    }
    void *visit(slang::FuncRef &p, void *ctx) final
    {
        return visit_type(p, static_cast<C *>(ctx));
    }
};

} // namespace slang

#endif //SLANG_IVISITOR_H
