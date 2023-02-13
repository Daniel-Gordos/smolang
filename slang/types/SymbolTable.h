#ifndef SLANG_SYMBOLTABLE_H
#define SLANG_SYMBOLTABLE_H

#include <map>
#include <memory>
#include <string>
#include <string_view>

namespace slang
{

class FuncSignature;
class VarRef;
class Type;

using TypeMap = std::map<std::string, std::unique_ptr<Type>, std::less<>>;

class SymbolTable
{
    struct VarInfo
    {
        VarRef *ref;
        Type *type;
    };

public:
    SymbolTable();
    ~SymbolTable();
    void open_scope();
    void close_scope();

    void enter_var(VarRef &, Type *);
    [[nodiscard]] VarInfo *retrieve_var(std::string_view) const;

    void enter_func(FuncSignature const &);
    [[nodiscard]] const FuncSignature *retrieve_func(std::string_view) const;

    [[nodiscard]] Type *retrieve_type(std::string_view);

    [[nodiscard]] TypeMap acquire_types();

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace slang

#endif //SLANG_SYMBOLTABLE_H
