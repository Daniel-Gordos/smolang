#ifndef SLANG_SOURCEFILE_H
#define SLANG_SOURCEFILE_H

#include <memory>
#include <string_view>
#include <vector>
namespace slang
{

class SourceFile
{
public:
    SourceFile(std::istream &, std::string);
    ~SourceFile();

    [[nodiscard]] std::string_view get_name() const;
    [[nodiscard]] std::vector<std::string> const &get_lines() const;
    [[nodiscard]] std::string const &get_line_by_num(unsigned long) const;
    void print_source(std::ostream &);

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace slang

#endif //SLANG_SOURCEFILE_H
