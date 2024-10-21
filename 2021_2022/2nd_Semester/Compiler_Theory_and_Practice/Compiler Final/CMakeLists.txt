cmake_minimum_required(VERSION 3.0.0)
project(TinyLang VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(compile main.cpp Lexer/Token.cpp Lexer/Lexer.cpp Helper/Helper.cpp Parser/TokenManager.cpp Parser/Parser.cpp Parser/AST.cpp Visitor/XMLVisitor.cpp Visitor/SAVisitor.cpp Visitor/IVisitor.cpp)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
