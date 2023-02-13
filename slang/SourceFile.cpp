#include "SourceFile.h"

#include <iostream>
#include <iterator>
#include <utility>
#include <vector>

namespace slang
{
struct LineIterator
{
    using iterator_category = std::input_iterator_tag;
    using value_type = std::string;
    using difference_type = std::ptrdiff_t;
    using reference = const value_type &;
    using pointer = const value_type *;

    LineIterator()
        : _input(nullptr)
    {}
    explicit LineIterator(std::istream &input)
        : _input(&input)
    {
        ++*this;
    }

    reference operator*() const { return _s; }
    pointer operator->() const { return &**this; }

    LineIterator &operator++()
    {
        if (!std::getline(*_input, _s))
            _input = nullptr;
        return *this;
    }

    LineIterator operator++(int)
    {
        auto copy(*this);
        ++*this;
        return copy;
    }

    friend bool operator==(const LineIterator &x, const LineIterator &y)
    {
        return x._input == y._input;
    }

    friend bool operator!=(const LineIterator &x, const LineIterator &y)
    {
        return !(x == y);
    }

private:
    std::istream *_input;
    std::string _s;
};

struct FileLines
{
    FileLines(std::istream &input)
        : input_(input)
    {}

    [[nodiscard]] LineIterator begin() const { return LineIterator(input_); }
    [[nodiscard]] static LineIterator end() { return {}; }

private:
    std::istream &input_;
};

class SourceFile::Impl
{
public:
    Impl(std::istream &in_file, std::string name)
    {
        auto f = FileLines(in_file);
        _lines = {f.begin(), f.end()};
        _name = std::move(name);
    }
    std::string_view get_name() { return _name; }
    std::vector<std::string> &get_lines() { return _lines; }
    std::string &get_line_by_num(unsigned long n) { return _lines.at(n - 1); }
    void print_source(std::ostream &os)
    {
        for (auto const &line: _lines)
            os << line << '\n';
    }

private:
    std::vector<std::string> _lines;
    std::string _name;
};

SourceFile::SourceFile(std::istream &in_file, std::string name)
    : _impl(std::make_unique<Impl>(in_file, std::move(name))){};
SourceFile::~SourceFile() = default;

std::string_view SourceFile::get_name() const { return _impl->get_name(); }
std::vector<std::string> const &SourceFile::get_lines() const
{
    return _impl->get_lines();
}
std::string const &SourceFile::get_line_by_num(unsigned long n) const
{
    return _impl->get_line_by_num(n);
}
void SourceFile::print_source(std::ostream &os) { _impl->print_source(os); }

} // namespace slang