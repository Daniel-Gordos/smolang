
#include "Lexer.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <regex>
#include <sstream>
#include <vector>

namespace slang
{

class Lexer::Impl
{
public:
    explicit Impl(SourceFile &src_file)
        : _src_file{src_file}
    {}

    static int get_change_in_indent(std::string const &line, int curr_indent_level)
    {
        std::smatch sig_whitespace_match;
        if (std::regex_search(line, sig_whitespace_match, _indent_rgx))
        {
            auto n_spaces = sig_whitespace_match[1].length();
            auto new_indent_level = n_spaces / _indent_size;
            return new_indent_level - curr_indent_level;
        }
        return 0;
    }

    static void apply_change_in_indent(
        std::vector<Token> &tokens, int curr_indent, int rel_indent_change,
        unsigned long line_no)
    {
        auto initial_spaces = curr_indent * _indent_size;
        if (rel_indent_change > 0)
        {
            for (auto i = 0; i < rel_indent_change; ++i)
            {
                tokens.emplace_back(
                    Token::Kind::INDENT,
                    TextPosition{
                        line_no, initial_spaces + i * _indent_size,
                        initial_spaces + (i + 1) * _indent_size},
                    "    ");
            }
        }
        else if (rel_indent_change < 0)
        {
            for (auto i = 0; i < -rel_indent_change; ++i)
            {
                tokens.emplace_back(
                    Token::Kind::DEDENT,
                    TextPosition{
                        line_no, initial_spaces - (i * _indent_size),
                        initial_spaces - (i * _indent_size + 1)},
                    "    ");
            }
        }
    }

    std::vector<Token> parse_tokens()
    {
        auto builder = std::stringstream{};
        auto it = _token_patterns.begin();
        builder << "(" << it++->second << ")";
        for (; it != _token_patterns.end(); ++it)
        {
            builder << "|(" << it->second << ")";
        }
        auto rgx = std::regex{builder.str(), std::regex_constants::ECMAScript};

        auto tokens = std::vector<Token>{};

        auto line_no = static_cast<unsigned long>(1);
        auto indent_level = 0;
        auto &lines = _src_file.get_lines();
        for (auto line_it = lines.begin(); line_it != lines.end(); ++line_it, ++line_no)
        {
            auto &curr_line = *line_it;
            auto n_tokens_on_line = 0;

            auto rel_indent_change = get_change_in_indent(curr_line, indent_level);
            apply_change_in_indent(tokens, indent_level, rel_indent_change, line_no);
            indent_level += rel_indent_change;

            auto matches_it =
                std::sregex_iterator{curr_line.begin(), curr_line.end(), rgx};
            auto matches_end = std::sregex_iterator{};
            for (; matches_it != matches_end; ++matches_it)
            {
                auto const &match = *matches_it;

                auto find_group_it = std::find_if(
                    std::next(match.begin()), match.end(),
                    [](auto const &m) { return m.matched; });

                auto submatch_idx =
                    std::distance(std::next(match.begin()), find_group_it);
                auto kind = _token_patterns[submatch_idx].first;

                if (kind == Token::Kind::_WHITESPACE)
                    continue;

                auto pos = TextPosition{
                    line_no, static_cast<unsigned long>(match.position()),
                    static_cast<unsigned long>(match.position() + match.length())};
                tokens.emplace_back(
                    kind, pos,
                    std::string_view{
                        curr_line.c_str() + match.position(),
                        static_cast<size_t>(match.length())});

                ++n_tokens_on_line;
            }

            // Newlines are only significant for non-empty lines
            if (n_tokens_on_line > 0)
            {
                tokens.emplace_back(
                    Token::Kind::NEWLINE, TextPosition{line_no, curr_line.length(), 0},
                    "");
            }
        }

        // Balance remaining indents
        for (auto i = 0; i < indent_level; ++i)
        {
            tokens.emplace_back(Token::Kind::DEDENT, TextPosition{line_no, 0, 0}, "    ");
        }

        tokens.emplace_back(Token::Kind::_EOF, TextPosition(line_no, 0, 0), "");

        return tokens;
    }

private:
    SourceFile &_src_file;
    static const std::vector<std::pair<Token::Kind, std::string>> _token_patterns;
    static const std::regex _indent_rgx;
    static const unsigned long _indent_size;
};

const std::regex Lexer::Impl::_indent_rgx = std::regex{R"(^( *)[^\s])"};
const unsigned long Lexer::Impl::_indent_size = 4;

const decltype(Lexer::Impl::_token_patterns) Lexer::Impl::_token_patterns = {
    {Token::LBRACKET, R"(\[)"},
    {Token::RBRACKET, R"(\])"},
    {Token::LPAREN, R"(\()"},
    {Token::RPAREN, R"(\))"},

    {Token::INFER, R"(:=)"},
    {Token::COLON, R"(:)"},
    {Token::COMMA, R"(,)"},

    {Token::EQEQ, R"(==)"},
    {Token::EQ, R"(=)"},
    {Token::NOTEQ, R"(!=)"},

    {Token::PLUS, R"(\+)"},
    {Token::MINUS, R"(\-)"},
    {Token::MULT, R"(\*)"},
    {Token::DIV, R"(/)"},

    {Token::GE, R"(>=)"},
    {Token::GT, R"(>)"},
    {Token::LE, R"(<=)"},
    {Token::LT, R"(<)"},

    {Token::FUN, R"(fun\b)"},
    {Token::RETURN, R"(return\b)"},
    {Token::PASS, R"(pass\b)"},
    {Token::CLASS, R"(class\b)"},
    {Token::MET, R"(met\b)"},
    {Token::IF, R"(if\b)"},
    {Token::FOR, R"(for\b)"},
    {Token::WHILE, R"(while\b)"},
    {Token::ELIF, R"(elif\b)"},
    {Token::ELSE, R"(else\b)"},
    {Token::AND, R"(and\b)"},
    {Token::OR, R"(or\b)"},
    {Token::NOT, R"(!)"},

    {Token::FLOAT_LIT, R"(\d+\.\d+)"},
    {Token::INT_LIT, R"(\d+)"},
    {Token::BOOL_LIT, R"(true\b|false\b)"},
    {Token::STR_LIT, R"("[^"\n]*")"},
    {Token::IDENT, R"([a-zA-Z_][a-zA-Z0-9_]*)"},

    {Token::_WHITESPACE, R"([ \t]+)"},
    {Token::_FAILED_PARSE, R"(.)"},
};

std::vector<Token> Lexer::parse_tokens() { return _impl->parse_tokens(); }
Lexer::Lexer(SourceFile &src_file)
    : _impl(std::make_unique<Impl>(src_file))
{}

Lexer::~Lexer() = default;

} // namespace slang
