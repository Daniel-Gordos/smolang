#include "ASTBuilder.h"
#include "Lexer.h"
#include "SourceFile.h"
#include "dependencies/args.h"
#include "visitors/IrGenerator.h"
#include "visitors/Printer.h"
#include "visitors/TypeChecker.h"
#include <fstream>
#include <iostream>

int main(int, char **argv)
{
    auto args = argh::parser{argv};

    if (args.size() < 2)
    {
        std::cerr << "Usage: " << args[0] << " <source_file> [--debug] [--check]\n";
        exit(1);
    }
    auto const &input_file = args[1];

    auto f = std::ifstream{input_file};
    if (!f.good())
    {
        std::cerr << "Failed to open file: " << input_file << "\n";
        exit(1);
    }

    auto src_file = slang::SourceFile{f, input_file};
    auto lexer = slang::Lexer{src_file};

    auto tokens = lexer.parse_tokens();

    try
    {
        auto parser = slang::ASTBuilder(tokens);
        auto ast = parser.build();

        slang::TypeChecker type_checker;
        ast->accept(type_checker, {});

        if (args["check"])
        {
            return 0;
        }

        if (args["debug"])
        {
            auto printer = slang::Printer{std::cout, 4};
            ast->accept(printer, {});
        }

        auto types = type_checker.get_defined_types();

        auto generator = slang::IRGenerator{types, true};
        ast->accept(generator, {});
        generator.print_ir();
    }
    catch (const slang::Exception &e)
    {
        std::cerr << e.what();
    }
}
