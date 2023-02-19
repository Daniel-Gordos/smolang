
#include "ASTBuilder.h"
#include "Token.h"
#include "expressions/AssignExpr.h"
#include "expressions/BinaryExpr.h"
#include "expressions/CallExpr.h"
#include "expressions/InferredDeclExpr.h"
#include "expressions/LiteralExpr.h"
#include "expressions/UnaryExpr.h"
#include "expressions/VarRefExpr.h"
#include "statements/ExprStmt.h"
#include "statements/IfStmt.h"
#include "statements/ReturnStmt.h"
#include "statements/StmtBlock.h"
#include "statements/WhileStmt.h"

#include <memory>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace slang
{

class ASTBuilder::Impl
{
private:
    Token const &curr_token() { return _tokens[_token_idx]; }

    bool curr_is(Token::Kind kind) { return curr_token().get_kind() == kind; }

    Token const &accept() { return _tokens[_token_idx++]; }

    bool accept_if(Token::Kind kind)
    {
        auto is_expected = curr_is(kind);
        if (is_expected)
            accept();
        return is_expected;
    }

    Token const &match(Token::Kind kind)
    {
        if (kind != curr_token().get_kind())
        {
            throw ASTParseException{curr_token(), kind};
        }
        return accept();
    }

    std::unique_ptr<Program> parse_program();
    std::unique_ptr<FuncDef> parse_func_def();
    std::unique_ptr<FuncSignature> parse_func_signature();
    std::unique_ptr<StmtBlock> parse_stmt_block();
    std::unique_ptr<Stmt> parse_stmt();
    std::unique_ptr<StmtList> parse_stmt_list();
    std::unique_ptr<FuncParamDef> parse_func_param_def();
    std::vector<std::unique_ptr<FuncParamDef>> parse_param_list();
    std::unique_ptr<ExprStmt> parse_expr_stmt();
    std::unique_ptr<Expr> parse_expr();
    std::unique_ptr<Expr> parse_expr_climbing(std::unique_ptr<Expr> &&, int);
    std::unique_ptr<Expr> parse_primary_expr();
    std::unique_ptr<IfStmt> parse_if_stmt();
    std::unique_ptr<IfStmt> parse_elif_stmt();
    std::unique_ptr<ReturnStmt> parse_return_stmt();
    std::unique_ptr<WhileStmt> parse_while_stmt();
    std::unique_ptr<CallExpr> parse_call_expr_tail(Token);

    static int precedence_of(Token const &);
    static std::unique_ptr<Expr> make_binary_expr_type(
        std::unique_ptr<Expr> &&, std::unique_ptr<Expr> &&, Token const &);

    std::vector<Token> const &_tokens;
    std::size_t _token_idx;
    static const std::unordered_map<Token::Kind, int> OP_PRECEDENCE;
    static const std::unordered_set<Token::Kind> RIGHT_ASSOCIATIVE;

public:
    explicit Impl(std::vector<Token> const &);
    std::unique_ptr<Program> build();
};

ASTBuilder::Impl::Impl(std::vector<Token> const &tokens)
    : _tokens{tokens}
    , _token_idx{0}
{}

const std::unordered_map<Token::Kind, int> ASTBuilder::Impl::OP_PRECEDENCE = {
    {Token::INFER, 10}, {Token::EQ, 10},    {Token::OR, 15},    {Token::AND, 15},
    {Token::EQEQ, 20},  {Token::NOTEQ, 20}, {Token::GE, 20},    {Token::GT, 20},
    {Token::LE, 20},    {Token::LT, 20},    {Token::MINUS, 30}, {Token::PLUS, 30},
    {Token::DIV, 40},   {Token::MULT, 40},
};

const std::unordered_set<Token::Kind> ASTBuilder::Impl::RIGHT_ASSOCIATIVE = {
    Token::INFER,
    Token::EQ,
    Token::PLUS,
    Token::MULT,
};

std::unique_ptr<Program> ASTBuilder::Impl::build() { return parse_program(); }

std::unique_ptr<Program> ASTBuilder::Impl::parse_program()
{
    auto program = std::make_unique<Program>();

    while (accept_if(Token::NEWLINE))
        ; // Skip empty lines between func defs

    while (curr_is(Token::FUN))
    {
        program->add_func(parse_func_def());

        while (accept_if(Token::NEWLINE))
            ;
    }

    match(Token::_EOF);
    return program;
}

std::unique_ptr<FuncDef> ASTBuilder::Impl::parse_func_def()
{
    auto signature = parse_func_signature();
    match(Token::NEWLINE);
    auto body = parse_stmt_block();
    return std::make_unique<FuncDef>(std::move(signature), std::move(body));
}

std::unique_ptr<FuncSignature> ASTBuilder::Impl::parse_func_signature()
{
    match(Token::FUN);
    auto func_name = std::make_unique<FuncRef>(match(Token::IDENT));
    match(Token::LPAREN);

    auto params = parse_param_list();

    match(Token::RPAREN);

    auto return_type = std::unique_ptr<TypeRef>{};
    if (!curr_is(Token::COLON))
    {
        return_type = std::make_unique<TypeRef>(match(Token::IDENT));
    }
    match(Token::COLON);

    auto sig = std::make_unique<FuncSignature>(
        std::move(return_type), std::move(func_name), std::move(params));

    return sig;
}
std::vector<std::unique_ptr<FuncParamDef>> ASTBuilder::Impl::parse_param_list()
{
    auto params = std::vector<std::unique_ptr<FuncParamDef>>{};

    // Cases:
    // foo()
    // foo(a)
    // foo(a,)
    // foo(a, b)
    // foo(a, b,) etc..

    if (!curr_is(Token::RPAREN))
        params.push_back(parse_func_param_def());

    while (!curr_is(Token::RPAREN))
    {
        match(Token::COMMA);
        if (curr_is(Token::RPAREN))
            break;

        params.push_back(parse_func_param_def());
    }

    return params;
}
std::unique_ptr<StmtBlock> ASTBuilder::Impl::parse_stmt_block()
{
    match(Token::INDENT);
    if (accept_if(Token::PASS))
    {
        match(Token::NEWLINE);
        match(Token::DEDENT);
        return {};
    }

    auto stmts = std::vector<std::unique_ptr<Stmt>>{};
    while (!curr_is(Token::DEDENT))
        stmts.emplace_back(parse_stmt());

    match(Token::DEDENT);

    return std::make_unique<StmtBlock>(std::move(stmts));
}

std::unique_ptr<WhileStmt> ASTBuilder::Impl::parse_while_stmt()
{
    match(Token::WHILE);
    auto expr = parse_expr();

    match(Token::COLON);
    match(Token::NEWLINE);
    auto body = parse_stmt_block();

    return std::make_unique<WhileStmt>(std::move(expr), std::move(body));
}

std::unique_ptr<Stmt> ASTBuilder::Impl::parse_stmt()
{
    switch (curr_token().get_kind())
    {
        case Token::IF:
            return parse_if_stmt();
        case Token::RETURN:
            return parse_return_stmt();
        case Token::WHILE:
            return parse_while_stmt();
        case Token::FOR:
            // TODO

        default:
            return parse_expr_stmt();
    }
}

std::unique_ptr<FuncParamDef> ASTBuilder::Impl::parse_func_param_def()
{

    auto type = std::make_unique<TypeRef>(match(Token::IDENT));
    auto name = std::make_unique<VarRef>(match(Token::IDENT));
    return std::make_unique<FuncParamDef>(std::move(type), std::move(name));
}

std::unique_ptr<ExprStmt> ASTBuilder::Impl::parse_expr_stmt()
{
    auto expr = parse_expr();
    match(Token::NEWLINE);
    return std::make_unique<ExprStmt>(std::move(expr));
}

std::unique_ptr<Expr> ASTBuilder::Impl::parse_expr()
{
    return parse_expr_climbing(parse_primary_expr(), -1);
}

std::unique_ptr<Expr>
ASTBuilder::Impl::parse_expr_climbing(std::unique_ptr<Expr> &&lhs, int min_precedence)
{
    auto lookahead = curr_token();
    while (OP_PRECEDENCE.contains(lookahead.get_kind()) &&
           OP_PRECEDENCE.at(lookahead.get_kind()) >= min_precedence)
    {
        auto op = accept();
        auto rhs = parse_primary_expr();
        lookahead = curr_token();
        while (OP_PRECEDENCE.contains(lookahead.get_kind()) &&
                   (precedence_of(lookahead) > precedence_of(op)) ||
               (RIGHT_ASSOCIATIVE.contains(lookahead.get_kind()) &&
                precedence_of(lookahead) == precedence_of(op)))
        {
            auto precedence = precedence_of(op) +
                              (precedence_of(lookahead) > precedence_of(op) ? 1 : 0);
            rhs = parse_expr_climbing(std::move(rhs), precedence);
            lookahead = curr_token();
        }

        lhs = make_binary_expr_type(std::move(lhs), std::move(rhs), op);
    }

    return lhs;
}

std::unique_ptr<Expr> ASTBuilder::Impl::parse_primary_expr()
{
    switch (curr_token().get_kind())
    {
        case Token::LPAREN:
        {
            accept();
            auto expr = parse_expr();
            match(Token::RPAREN);
            return expr;
        }

        case Token::INT_LIT:
        case Token::STR_LIT:
        case Token::BOOL_LIT:
        case Token::FLOAT_LIT:
            return std::make_unique<LiteralExpr>(accept());

        case Token::IDENT:
        {
            auto tok = accept();
            if (curr_is(Token::LPAREN))
            {
                return parse_call_expr_tail(tok);
            }
            else
            {
                return std::make_unique<VarRefExpr>(std::make_unique<VarRef>(tok));
            }
        }

        case Token::PLUS:
        case Token::MINUS:
        {
            auto op = accept();
            return std::make_unique<UnaryExpr>(parse_primary_expr(), op);
        }
        default:
            throw ASTParseException{curr_token(), "Expected primary expression"};
    }
}
int ASTBuilder::Impl::precedence_of(const Token &tok)
{
    return OP_PRECEDENCE.at(tok.get_kind());
}
std::unique_ptr<Expr> ASTBuilder::Impl::make_binary_expr_type(
    std::unique_ptr<Expr> &&lhs, std::unique_ptr<Expr> &&rhs, const Token &op)
{
    switch (op.get_kind())
    {
        case Token::EQ:
            return std::make_unique<AssignExpr>(std::move(lhs), std::move(rhs));

        case Token::INFER:
        {

            if (!dynamic_cast<VarRefExpr *>(lhs.get()))
            {
                throw ASTParseException{
                    op, "LHS of an inferred declaration must be a var ref expression."};
            }
            auto var_ptr =
                std::unique_ptr<VarRefExpr>(dynamic_cast<VarRefExpr *>(lhs.release()));
            auto t = var_ptr->get_type();
            return std::make_unique<InferredDeclExpr>(std::move(var_ptr), std::move(rhs));
        }

        default:
            return std::make_unique<BinaryExpr>(std::move(lhs), std::move(rhs), op);
    }
}
std::unique_ptr<CallExpr> ASTBuilder::Impl::parse_call_expr_tail(Token name)
{
    auto args = std::vector<std::unique_ptr<Expr>>{};
    match(Token::LPAREN);

    if (!curr_is(Token::RPAREN))
        args.emplace_back(parse_expr());

    while (!curr_is(Token::RPAREN))
    {
        match(Token::COMMA);
        if (curr_is(Token::RPAREN))
            break;

        args.emplace_back(parse_expr());
    }
    match(Token::RPAREN);

    return std::make_unique<CallExpr>(std::make_unique<FuncRef>(name), std::move(args));
}
std::unique_ptr<IfStmt> ASTBuilder::Impl::parse_if_stmt()
{
    match(Token::IF);
    auto expr = parse_expr();
    match(Token::COLON);
    match(Token::NEWLINE);

    auto body = parse_stmt_block();

    std::unique_ptr<StmtBlock> else_block;
    if (curr_is(Token::ELIF))
    {
        else_block = std::make_unique<StmtBlock>(std::vector<std::unique_ptr<Stmt>>{});
        else_block->add_stmt(parse_elif_stmt());
    }
    else if (accept_if(Token::ELSE))
    {
        match(Token::COLON);
        match(Token::NEWLINE);
        else_block = parse_stmt_block();
    }

    return std::make_unique<IfStmt>(
        std::move(expr), std::move(body), std::move(else_block));
}

std::unique_ptr<IfStmt> ASTBuilder::Impl::parse_elif_stmt()
{
    match(Token::ELIF);
    auto expr = parse_expr();
    match(Token::COLON);
    match(Token::NEWLINE);

    auto body = parse_stmt_block();

    std::unique_ptr<StmtBlock> else_block;
    if (curr_is(Token::ELIF))
    {
        else_block = std::make_unique<StmtBlock>(std::vector<std::unique_ptr<Stmt>>{});
        else_block->add_stmt(parse_elif_stmt());
    }
    else if (accept_if(Token::ELSE))
    {
        match(Token::COLON);
        match(Token::NEWLINE);
        else_block = parse_stmt_block();
    }

    return std::make_unique<IfStmt>(
        std::move(expr), std::move(body), std::move(else_block));
}

std::unique_ptr<ReturnStmt> ASTBuilder::Impl::parse_return_stmt()
{
    match(Token::RETURN);
    auto expr = accept_if(Token::NEWLINE) ? std::unique_ptr<Expr>{} : parse_expr();
    if (expr)
        match(Token::NEWLINE);

    return std::make_unique<ReturnStmt>(std::move(expr));
}

ASTBuilder::ASTBuilder(const std::vector<Token> &tokens)
    : _impl{std::make_unique<Impl>(tokens)}
{}

ASTBuilder::~ASTBuilder() = default;

std::unique_ptr<Program> ASTBuilder::build() { return _impl->build(); }

ASTParseException::ASTParseException(const Token &got, Token::Kind expected)
{
    auto buf = std::stringstream{};
    buf << got.get_pos() << " : "
        << "Expected " << Token::kind_as_str(expected) << ", Got "
        << Token::kind_as_str(got.get_kind()) << "\n";
    _msg = buf.str();
}

const char *ASTParseException::what() const noexcept { return _msg.c_str(); }

ASTParseException::ASTParseException(Token const &got, std::string_view msg)
{
    auto buf = std::stringstream{};
    buf << got.get_pos() << ": " << msg << "\n";
    _msg = buf.str();
}

} // namespace slang
