#include "SymbolTable.h"
#include "Type.h"
#include "VarRef.h"
#include "functions/FuncSignature.h"
#include <map>
#include <string_view>
#include <vector>

#include "types/primitives/BoolType.h"
#include "types/primitives/FloatType.h"
#include "types/primitives/IntType.h"

namespace slang
{

class SymbolTable::Impl
{

    class Scope
    {
    public:
        void enter_var(VarRef &var, Type &type)
        {
            _variables.emplace(
                std::string(var.get_token().get_spelling()), VarInfo{&var, &type});
        }

        VarInfo *retrieve_var(std::string_view &name)
        {
            auto it = _variables.find(name);
            if (it == _variables.end())
                return nullptr;
            return &it->second;
        }

    private:
        std::map<std::string, VarInfo, std::less<>> _variables{};
    };

public:
    Impl()
    {
        _types["bool"] = std::make_unique<BoolType>();
        _types["float"] = std::make_unique<FloatType>();
        _types["int"] = std::make_unique<IntType>();

        _funcs = {};
    }

    void open_scope() { _scopes.emplace_back(); }

    void close_scope() { _scopes.pop_back(); }

    void enter_var(VarRef &var, Type &type) { _scopes.back().enter_var(var, type); }

    [[nodiscard]] VarInfo *retrieve_var(std::string_view &name)
    {
        for (auto it = _scopes.rbegin(); it != _scopes.rend(); ++it)
        {
            auto const v = it->retrieve_var(name);
            if (v != nullptr)
                return v;
        }
        return nullptr;
    }

    void enter_func(FuncSignature &sig)
    {
        _funcs.emplace(std::string(sig.get_name()->get_token().get_spelling()), &sig);
    }

    [[nodiscard]] FuncSignature *retrieve_func(std::string_view name) const
    {
        auto it = _funcs.find(name);
        if (it == _funcs.end())
            return nullptr;
        return it->second;
    }

    [[nodiscard]] Type *retrieve_type(std::string_view &name) const
    {
        auto it = _types.find(std::string(name));
        if (it == _types.end())
            return nullptr;
        return it->second.get();
    }

    [[nodiscard]] TypeMap acquire_types() { return std::move(_types); }

private:
    std::vector<Scope> _scopes{};
    std::map<std::string, FuncSignature *, std::less<>> _funcs;
    TypeMap _types{};
};

void SymbolTable::open_scope() { _impl->open_scope(); }
void SymbolTable::close_scope() { _impl->close_scope(); }
void SymbolTable::enter_var(VarRef &var, Type *type) { _impl->enter_var(var, *type); }
SymbolTable::VarInfo *SymbolTable::retrieve_var(std::string_view name) const
{
    return _impl->retrieve_var(name);
}
void SymbolTable::enter_func(FuncSignature &sig) { _impl->enter_func(sig); }
FuncSignature *SymbolTable::retrieve_func(std::string_view name) const
{
    return _impl->retrieve_func(name);
}
SymbolTable::SymbolTable()
    : _impl{std::make_unique<Impl>()}
{}
Type *SymbolTable::retrieve_type(std::string_view name)
{
    return _impl->retrieve_type(name);
}
TypeMap SymbolTable::acquire_types() { return _impl->acquire_types(); }

SymbolTable::~SymbolTable() = default;
} // namespace slang