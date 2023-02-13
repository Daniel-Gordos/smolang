
#include "Printer.h"
#include "Program.h"
#include "Type.h"
#include "expressions/AssignExpr.h"
#include "expressions/BinaryExpr.h"
#include "expressions/CallArgList.h"
#include "expressions/CallExpr.h"
#include "expressions/DerefExpr.h"
#include "expressions/InferredDeclExpr.h"
#include "expressions/LiteralExpr.h"
#include "expressions/UnaryExpr.h"
#include "expressions/VarRefExpr.h"
#include "functions/FuncBody.h"
#include "statements/ExprStmt.h"
#include "statements/IfStmt.h"
#include "statements/ReturnStmt.h"
#include "statements/WhileStmt.h"
#include <sstream>

namespace slang
{

Printer::Printer(std::ostream &os, int indent_size)
    : _os{os}
    , _indent_level{0}
{
    auto buf = std::stringstream{};
    for (auto i = 0; i < indent_size; ++i)
        buf << " ";
    _indent_str = buf.str();
}

std::ostream &Printer::indent()
{
    for (auto i = 0; i < _indent_level; ++i)
        _os << _indent_str;
    return _os;
}

std::string_view show_type(Type *type)
{
    if (type)
        return type->get_name();
    return "void";
}

void *Printer::visit_type(Program &program, void *ctx)
{
    indent() << "<Program>\n";
    ++_indent_level;
    program.func_def_list->accept(*this, {});
    --_indent_level;
    indent() << "</Program>\n";
    return {};
}
void *Printer::visit_type(FuncDef &def, void *ctx)
{
    indent() << "<FuncDef>\n";
    ++_indent_level;
    def.get_signature()->accept(*this, {});
    def.get_body()->accept(*this, {});
    --_indent_level;
    indent() << "</FuncDef>\n";
    return {};
}
void *Printer::visit_type(FuncDefList &list, void *ctx)
{
    indent() << "<FuncDefList>\n";
    ++_indent_level;
    list.get_func_def()->accept(*this, {});
    if (list.get_next())
        list.get_next()->accept(*this, {});
    --_indent_level;
    indent() << "</FuncDefList>\n";
    return {};
}
void *Printer::visit_type(FuncBody &body, void *c)
{
    indent() << "<FuncBody>\n";
    ++_indent_level;
    if (body.get_statements())
        body.get_statements()->accept(*this, {});
    --_indent_level;
    indent() << "</FuncBody>\n";
    return {};
}
void *Printer::visit_type(FuncSignature &signature, void *ctx)
{
    auto name = signature.get_name()->get_token().get_spelling();
    indent() << "<FuncSignature name=\"" << name << "\">\n";
    ++_indent_level;
    if (signature.get_return_type())
        signature.get_return_type()->accept(*this, {});
    if (signature.get_params())
        signature.get_params()->accept(*this, {});
    --_indent_level;
    indent() << "</FuncSignature>\n";
    return {};
}
void *Printer::visit_type(StmtList &list, void *ctx)
{
    indent() << "<StmtList>\n";
    ++_indent_level;
    list.get_statement()->accept(*this, {});
    if (list.get_next())
        list.get_next()->accept(*this, {});
    --_indent_level;
    indent() << "</StmtList>\n";
    return {};
}
void *Printer::visit_type(FuncParamDefList &list, void *ctx)
{
    indent() << "<FuncParamDefList>\n";
    ++_indent_level;
    list.get_param()->accept(*this, {});
    if (list.get_next())
        list.get_next()->accept(*this, {});
    --_indent_level;
    indent() << "</FuncParamDefList>\n";
    return {};
}
void *Printer::visit_type(FuncParamDef &def, void *ctx)
{
    auto name = def.get_var()->get_token().get_spelling();
    indent() << "<FuncParamDef name=" << name << "\">\n";
    ++_indent_level;
    def.get_type()->accept(*this, {});
    --_indent_level;
    indent() << "</FuncParamDef>\n";
    return {};
}
void *Printer::visit_type(TypeRef &type, void *ctx)
{
    indent() << "<TypeRef name=\"" << type.get_token().get_spelling() << "\"/>\n";
    return {};
}
void *Printer::visit_type(VarRef &var, void *ctx)
{
    indent() << "<VarRef name=\"" << var.get_token().get_spelling() << "\"/>\n";
    return {};
}
void *Printer::visit_type(InferredDeclExpr &expr, void *c)
{
    indent() << "<InferredDeclExpr type=" << show_type(expr.get_type()) << ">\n";
    ++_indent_level;
    expr.get_lhs()->accept(*this, {});
    expr.get_rhs()->accept(*this, {});
    --_indent_level;
    indent() << "</InferredDeclExpr>\n";
    return {};
}
void *Printer::visit_type(ExprStmt &stmt, void *c)
{
    indent() << "<ExprStmt>\n";
    ++_indent_level;
    stmt.get_expr()->accept(*this, {});
    --_indent_level;
    indent() << "</ExprStmt>\n";
    return {};
}
void *Printer::visit_type(BinaryExpr &expr, void *c)
{
    indent() << "<BinaryExpr op=\"" << expr.get_op().get_spelling()
             << "\" type=" << show_type(expr.get_type()) << ">\n";
    ++_indent_level;
    expr.get_lhs()->accept(*this, {});
    expr.get_rhs()->accept(*this, {});
    --_indent_level;
    indent() << "</BinaryExpr>\n";
    return {};
}
void *Printer::visit_type(UnaryExpr &expr, void *c)
{
    indent() << "<UnaryExpr op=\"" << expr.get_op().get_spelling()
             << "\" type=" << show_type(expr.get_type()) << ">\n";
    ++_indent_level;
    expr.get_expr()->accept(*this, {});
    --_indent_level;
    indent() << "</UnaryExpr>\n";
    return {};
}
void *Printer::visit_type(VarRefExpr &expr, void *c)
{
    indent() << "<VarRefExpr type=" << show_type(expr.get_type()) << ">\n";
    ++_indent_level;
    expr.get_var()->accept(*this, {});
    --_indent_level;
    indent() << "</VarRefExpr>\n";
    return {};
}
void *Printer::visit_type(DerefExpr &expr, void *c)
{
    indent() << "<DerefExpr>\n";
    ++_indent_level;
    expr.get_ref()->accept(*this, {});
    --_indent_level;
    indent() << "</DerefExpr>\n";
    return {};
}
void *Printer::visit_type(LiteralExpr &expr, void *c)
{
    indent() << "<LiteralExpr kind=" << Token::kind_as_str(expr.get_token().get_kind())
             << " spelling=\"" << expr.get_token().get_spelling()
             << "\" type=" << show_type(expr.get_type()) << "/>\n";
    return {};
}
void *Printer::visit_type(AssignExpr &expr, void *c)
{
    indent() << "<AssignExpr type=" << show_type(expr.get_type()) << ">\n";
    ++_indent_level;
    expr.get_lhs()->accept(*this, {});
    expr.get_rhs()->accept(*this, {});
    --_indent_level;
    indent() << "</AssignExpr>\n";
    return {};
}
void *Printer::visit_type(IfStmt &stmt, void *c)
{
    indent() << "<IfStmt>\n";
    ++_indent_level;
    stmt.get_expr()->accept(*this, {});
    if (stmt.get_stmts())
        stmt.get_stmts()->accept(*this, {});
    if (stmt.get_else_stmts())
        stmt.get_else_stmts()->accept(*this, {});
    --_indent_level;
    indent() << "</IfStmt>\n";
    return {};
}
void *Printer::visit_type(ReturnStmt &stmt, void *ctx)
{
    indent() << "<ReturnStmt>\n";
    ++_indent_level;
    if (stmt.get_expr())
        stmt.get_expr()->accept(*this, {});
    --_indent_level;
    indent() << "</ReturnStmt>\n";
    return {};
}
void *Printer::visit_type(CallExpr &expr, void *c)
{
    indent() << "<CallExpr name=\"" << expr.get_name()->get_token().get_spelling()
             << "\" type=" << show_type(expr.get_type()) << ">\n";
    ++_indent_level;
    if (expr.get_args())
        expr.get_args()->accept(*this, {});
    --_indent_level;
    indent() << "</CallExpr>\n";
    return {};
}
void *Printer::visit_type(CallArgList &list, void *c)
{
    indent() << "<CallArgList>\n";
    ++_indent_level;
    list.get_arg()->accept(*this, {});
    if (list.get_next())
        list.get_next()->accept(*this, {});
    --_indent_level;
    indent() << "</CallArgList>\n";
    return {};
}
void *Printer::visit_type(WhileStmt &stmt, void *c)
{
    indent() << "<WhileStmt>\n";
    ++_indent_level;
    stmt.get_expr()->accept(*this, {});
    if (stmt.get_stmts())
        stmt.get_stmts()->accept(*this, {});
    --_indent_level;
    indent() << "</WhileStmt>\n";
    return {};
}
} // namespace slang