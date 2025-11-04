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

add_library(libcli STATIC CLI/CLILib.cpp)

target_include_directories(libcli PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(chtl
    CLI/main.cpp
)

target_link_libraries(chtl
    PRIVATE
        libcli
)

target_link_libraries(chtl
    PRIVATE
        libchtl
        libchtljs
        libsharedcore
)
