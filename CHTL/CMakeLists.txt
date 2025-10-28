add_library(CHTLLexer STATIC Lexer.cpp)
target_include_directories(CHTLLexer PUBLIC ${CMAKE_SOURCE_DIR})

add_library(CHTLParser STATIC CHTLParser/Parser.cpp)
target_include_directories(CHTLParser PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(CHTLParser PRIVATE CHTLLexer)
