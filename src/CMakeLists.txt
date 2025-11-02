add_library(libchtl)

target_sources(libchtl
  PRIVATE
    CHTL/CHTLLexer/Lexer.cpp
    CHTL/CHTLParser/Parser.cpp
    CHTL/CHTLGenerator/Generator.cpp
)

target_include_directories(libchtl
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
