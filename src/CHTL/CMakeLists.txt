add_library(libchtl STATIC)

target_sources(libchtl
    PRIVATE
    CHTLLexer/Lexer.cpp
    CHTLParser/Parser.cpp
    CHTLGenerator/Generator.cpp
)

target_include_directories(libchtl
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/src/SharedCore
)
