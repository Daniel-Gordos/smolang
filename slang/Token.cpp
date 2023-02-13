//
// Created by Danny Gordos on 2/11/2022.
//

#include "Token.h"

#include <ostream>
#include <utility>

namespace slang
{

Token::Token(Token::Kind kind, TextPosition pos, std::string_view text)
    : _kind{kind}
    , _pos{pos}
    , _spelling{text}
{}

Token::Kind Token::get_kind() const { return _kind; }

TextPosition Token::get_pos() const { return _pos; }

std::string_view const &Token::get_spelling() const { return _spelling; }

std::string Token::kind_as_str(Token::Kind kind)
{
    switch (kind)
    {
        case LBRACKET:
            return "LBRACKET";
        case RBRACKET:
            return "RBRACKET";
        case LPAREN:
            return "LPAREN";
        case RPAREN:
            return "RPAREN";
        case INDENT:
            return "INDENT";
        case DEDENT:
            return "DEDENT";
        case COLON:
            return "COLON";
        case IDENT:
            return "IDENT";
        case INT_LIT:
            return "INT_LIT";
        case FLOAT_LIT:
            return "FLOAT_LIT";
        case BOOL_LIT:
            return "BOOL_LIT";
        case STR_LIT:
            return "STR_LIT";
        case EQ:
            return "EQ";
        case EQEQ:
            return "EQEQ";
        case LT:
            return "LT";
        case LE:
            return "LE";
        case GT:
            return "GT";
        case GE:
            return "GE";
        case PLUS:
            return "PLUS";
        case MINUS:
            return "MINUS";
        case MULT:
            return "MULT";
        case DIV:
            return "DIV";
        case FUN:
            return "FUN";
        case IF:
            return "IF";
        case ELIF:
            return "ELIF";
        case FOR:
            return "FOR";
        case NOTEQ:
            return "NOTEQ";
        case COMMA:
            return "COMMA";
        case ELSE:
            return "ELSE";
        case WHILE:
            return "WHILE";
        case _FAILED_PARSE:
            return "FAILED_PARSE";
        case _WHITESPACE:
            return "WHITESPACE";
        case NEWLINE:
            return "NEWLINE";
        case _EOF:
            return "EOF";
        case MET:
            return "MET";
        case RETURN:
            return "RETURN";
        case PASS:
            return "PASS";
        case CLASS:
            return "CLASS";
        case INFER:
            return "INFER";
        case AND:
            return "AND";
        case OR:
            return "OR";
    }
}

std::ostream &operator<<(std::ostream &os, const Token &t)
{
    return os << "Token{" << Token::kind_as_str(t.get_kind()) << ", \""
              << t.get_spelling() << "\", " << t.get_pos() << "}";
}

TextPosition::TextPosition(
    unsigned long start_line, unsigned long end_line, unsigned long start_offset,
    unsigned long end_offset)
    : start_line{start_line}
    , end_line{end_line}
    , start_offset{start_offset}
    , end_offset{end_offset}
{}

TextPosition::TextPosition(
    unsigned long line, unsigned long start_offset, unsigned long end_offset)
    : TextPosition(line, line, start_offset, end_offset)
{}

std::ostream &operator<<(std::ostream &os, const TextPosition &p)
{
    if (p.start_line == p.end_line)
    {
        return os << "Pos{" << p.start_line << ", " << p.start_offset << " - "
                  << p.end_offset << "}";
    }
    else
    {
        return os << "Pos{" << p.start_line << ", " << p.start_offset << " - "
                  << p.end_line << ", " << p.end_offset << "}";
    }
}
TextPosition::TextPosition() = default;

} // namespace slang
