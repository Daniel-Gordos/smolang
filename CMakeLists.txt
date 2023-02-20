cmake_minimum_required(VERSION 3.23)
project(slang)

set(CMAKE_CXX_STANDARD 20)

# LLVM Stuff
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "Include dirs: ${LLVM_INCLUDE_DIRS}")
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
llvm_map_components_to_libnames(llvm_libs support core irreader)


add_library(tokens STATIC slang/Token.cpp slang/Exception.h)
add_library(lexer STATIC slang/Lexer.cpp slang/SourceFile.cpp slang/SourceFile.h slang/Exception.h)

add_library(ast STATIC
        slang/ast/ASTNode.h
        slang/ast/Program.cpp
        slang/ast/functions/FuncDef.cpp
        slang/ast/functions/FuncSignature.cpp
        slang/ASTBuilder.cpp
        slang/ast/TypeRef.cpp
        slang/ast/TypeRef.h
        slang/ast/functions/FuncParamDef.cpp
        slang/ast/functions/FuncParamDef.h
        slang/ast/VarRef.cpp
        slang/ast/VarRef.h
        slang/visitors/IVisitor.h
        slang/visitors/Printer.cpp
        slang/visitors/Printer.h
        slang/ast/statements/Stmt.cpp
        slang/ast/expressions/Expr.h
        slang/ast/expressions/InferredDeclExpr.cpp
        slang/ast/expressions/InferredDeclExpr.h
        slang/ast/statements/ExprStmt.cpp
        slang/ast/statements/ExprStmt.h
        slang/ast/expressions/BinaryExpr.cpp
        slang/ast/expressions/BinaryExpr.h
        slang/ast/expressions/RefExpr.cpp
        slang/ast/expressions/RefExpr.h
        slang/ast/expressions/VarRefExpr.cpp
        slang/ast/expressions/VarRefExpr.h
        slang/ast/expressions/DerefExpr.cpp
        slang/ast/expressions/DerefExpr.h
        slang/ast/expressions/UnaryExpr.cpp
        slang/ast/expressions/UnaryExpr.h
        slang/ast/expressions/LiteralExpr.cpp
        slang/ast/expressions/LiteralExpr.h
        slang/ast/expressions/AssignExpr.cpp
        slang/ast/expressions/AssignExpr.h
        slang/ast/statements/IfStmt.cpp
        slang/ast/statements/IfStmt.h
        slang/ast/statements/ReturnStmt.cpp
        slang/ast/statements/ReturnStmt.h
        slang/ast/expressions/CallExpr.cpp
        slang/ast/expressions/CallExpr.h
        slang/ast/Symbol.h slang/ast/functions/FuncRef.cpp slang/ast/functions/FuncRef.h slang/Exception.h slang/ast/statements/WhileStmt.cpp slang/ast/statements/WhileStmt.h slang/ast/statements/StmtBlock.cpp slang/ast/statements/StmtBlock.h slang/dependencies/args.h)

add_library(typing STATIC
        slang/types/Type.cpp slang/types/Type.h slang/types/SymbolTable.cpp slang/types/SymbolTable.h slang/visitors/TypeChecker.cpp slang/visitors/TypeChecker.h slang/Exception.h slang/ast/statements/WhileStmt.cpp slang/ast/statements/WhileStmt.h slang/types/primitives/BoolType.cpp slang/types/primitives/BoolType.h slang/types/primitives/IntType.cpp slang/types/primitives/IntType.h slang/types/primitives/FloatType.cpp slang/types/primitives/FloatType.h slang/ast/statements/StmtBlock.cpp slang/ast/statements/StmtBlock.h)

add_executable(slang slang/main.cpp slang/SourceFile.cpp slang/SourceFile.h slang/visitors/IrGenerator.cpp slang/visitors/IrGenerator.h slang/Exception.h)


include_directories(slang slang/ast slang/visitors slang/types)

target_link_libraries(lexer tokens)
target_link_libraries(ast tokens)
target_link_libraries(typing ast)


target_link_libraries(slang lexer ast typing ${llvm_libs} ${LLVM_LDFLAGS})
