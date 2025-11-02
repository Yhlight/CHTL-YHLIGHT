add_library(libchtl)

target_sources(libchtl
  PRIVATE
    CHTL/CHTLLexer/Lexer.cpp
    CHTL/CHTLParser/Parser.cpp
    CHTL/CHTLGenerator/Generator.cpp
    CHTL/CHTLAnalyser/Analyser.cpp
    CHTL/CHTLImporter/Importer.cpp
)

target_include_directories(libchtl
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(SharedCore)
add_subdirectory(CHTLJS)

target_link_libraries(libchtl
    PRIVATE
        libsharedcore
)
