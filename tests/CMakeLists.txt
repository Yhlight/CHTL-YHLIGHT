add_executable(run_tests main.cpp LexerTest.cpp ParserTest.cpp GeneratorTest.cpp)

target_link_libraries(run_tests
    GTest::gtest_main
    libchtl
)

include(GoogleTest)
gtest_discover_tests(run_tests)
