#ifndef SLANG_EXCEPTION_H
#define SLANG_EXCEPTION_H

#include <exception>

namespace slang
{

// Base class for compiler exceptions
class Exception : public std::exception
{};

} // namespace slang

#endif //SLANG_EXCEPTION_H
