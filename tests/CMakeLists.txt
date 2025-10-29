include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(
  run_tests
  TokenTest/LexerTest.cpp
  AstTest/ParserTest.cpp
  GeneratorTest.cpp
  StyleTest.cpp
  ArithmeticTest.cpp
  ReferenceTest.cpp
  ConditionalTest.cpp
  TemplateTest.cpp
  CustomTest.cpp
  ImportTest.cpp
  NamespaceTest.cpp
  ../src/CHTL/CHTLLexer/Lexer.cpp
  ../src/CHTL/CHTLParser/Parser.cpp
  ../src/CHTL/CHTLGenerator/Generator.cpp
  ../src/CHTL/CHTLGenerator/ExpressionEvaluator.cpp
  ../src/CHTL/CHTLAnalyser/Analyser.cpp
  ../src/CHTL/CHTLAnalyser/SymbolTable.cpp
  ../src/CHTL/CHTLImporter/Importer.cpp
)

target_include_directories(run_tests PRIVATE ../src)

target_link_libraries(
  run_tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(run_tests)
