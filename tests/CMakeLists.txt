include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(
  run_tests
  TokenTest/LexerTest.cpp
  ../src/CHTL/CHTLLexer/Lexer.cpp
)

target_include_directories(run_tests PRIVATE ../src)

target_link_libraries(
  run_tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(run_tests)
