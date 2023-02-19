
#include "Program.h"

namespace slang
{
void *Program::accept(IVisitor &v, void *ctx) { return v.visit(*this, ctx); }
std::vector<std::unique_ptr<FuncDef>>::iterator Program::begin()
{
    return _funcs.begin();
}
std::vector<std::unique_ptr<FuncDef>>::iterator Program::end() { return _funcs.end(); }
void Program::add_func(std::unique_ptr<FuncDef> func)
{
    _funcs.push_back(std::move(func));
}

} // namespace slang