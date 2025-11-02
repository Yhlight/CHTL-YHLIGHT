include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(run_tests
  main.cpp
  LexerTest.cpp
  ParserTest.cpp
  GeneratorTest.cpp
    AnalyserTest.cpp
    ImporterTest.cpp
    Parser/ConfigurationTest.cpp
    Analyser/ConfigurationTest.cpp
    CHTLJS/LexerTest.cpp
    CHTLJS/ParserTest.cpp
    CHTLJS/GeneratorTest.cpp
)

target_link_libraries(run_tests
  PRIVATE
  libchtl
  libchtljs
  GTest::gtest
)

include(GoogleTest)
gtest_discover_tests(run_tests)
