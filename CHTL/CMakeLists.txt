cmake_minimum_required(VERSION 3.10)
project(CHTL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(chtl_compiler Main.cpp)

# Enable testing
enable_testing()

# Add lexer test
add_executable(lexer_test Test/LexerTest.cpp CHTLLexer/Lexer.cpp)
add_test(NAME LexerTest COMMAND lexer_test)

# Add parser test
add_executable(parser_test Test/ParserTest.cpp CHTLParser/Parser.cpp CHTLLexer/Lexer.cpp)
add_test(NAME ParserTest COMMAND parser_test)
