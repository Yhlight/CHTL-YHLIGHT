add_executable(chtl
    main.cpp
    CHTL/CHTLLexer/Lexer.cpp
    CHTL/CHTLParser/Parser.cpp
    CHTL/CHTLGenerator/Generator.cpp
    CHTL/CHTLGenerator/ExpressionEvaluator.cpp
    CHTL/CHTLAnalyser/Analyser.cpp
    CHTL/CHTLAnalyser/SymbolTable.cpp
    CHTL/CHTLImporter/Importer.cpp
)

target_include_directories(chtl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
