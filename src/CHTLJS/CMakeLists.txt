add_library(libchtljs)

target_sources(libchtljs
  PRIVATE
    CHTLLexer/Lexer.cpp
    CHTLParser/Parser.cpp
    CHTLGenerator/Generator.cpp
)

target_include_directories(libchtljs
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
