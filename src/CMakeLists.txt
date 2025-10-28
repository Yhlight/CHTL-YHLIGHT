add_executable(chtl
    main.cpp
    CHTL/CHTLLexer/Lexer.cpp
    CHTL/CHTLParser/Parser.cpp
)

target_include_directories(chtl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
