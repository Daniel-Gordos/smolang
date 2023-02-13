
#include "Program.h"

namespace slang
{
Program::Program(std::unique_ptr<FuncDefList> &&defs)
    : func_def_list{std::move(defs)}
{}
void *Program::accept(IVisitor &v, void *ctx) { return v.visit(*this, ctx); }

} // namespace slang