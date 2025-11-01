add_executable(run_tests
    dummy_test.cpp
    LexerTest.cpp
)

target_link_libraries(run_tests PRIVATE GTest::gtest_main libchtl)

include(GoogleTest)
gtest_discover_tests(run_tests)
