
#ifndef SLANG_TOKEN_H
#define SLANG_TOKEN_H

#include <ostream>
#include <string_view>

namespace slang
{

struct TextPosition
{
    unsigned long start_line{}, end_line{};
    unsigned long start_offset{}, end_offset{};

    TextPosition();
    TextPosition(
        unsigned long line, unsigned long start_offset, unsigned long end_offset);
    TextPosition(
        unsigned long start_line, unsigned long end_line, unsigned long start_offset,
        unsigned long end_offset);

    friend std::ostream &operator<<(std::ostream &os, const TextPosition &position);
};

class Token
{
public:
    enum Kind
    {
        LBRACKET,
        RBRACKET,
        LPAREN,
        RPAREN,

        INDENT,
        DEDENT,
        NEWLINE,

        COLON,
        COMMA,

        EQ,
        INFER,
        EQEQ,
        NOTEQ,

        LT,
        LE,
        GT,
        GE,

        PLUS,
        MINUS,
        MULT,
        DIV,

        AND,
        OR,
        NOT,

        FUN,
        MET,
        IF,
        ELIF,
        ELSE,
        FOR,
        WHILE,
        RETURN,
        CLASS,
        PASS,

        INT_LIT,
        FLOAT_LIT,
        BOOL_LIT,
        STR_LIT,
        IDENT,

        _FAILED_PARSE,
        _WHITESPACE,
        _EOF,

    };

    static std::string kind_as_str(Kind);

    Token(Kind, TextPosition, std::string_view);

    [[nodiscard]] TextPosition get_pos() const;

    [[nodiscard]] Kind get_kind() const;

    [[nodiscard]] std::string_view const &get_spelling() const;

    friend std::ostream &operator<<(std::ostream &, Token const &);

private:
    Kind _kind;
    TextPosition _pos;
    std::string_view _spelling;
};

} // namespace slang

#endif //SLANG_TOKEN_H
