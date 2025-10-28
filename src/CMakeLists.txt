add_executable(chtl
    main.cpp
    CHTL/CHTLLexer/Lexer.cpp
    CHTL/CHTLParser/Parser.cpp
    CHTL/CHTLGenerator/Generator.cpp
)

target_include_directories(chtl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
