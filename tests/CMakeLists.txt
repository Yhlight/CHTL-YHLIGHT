add_executable(chttl_tests
    LexerTest.cpp
    ParserTest.cpp
    GeneratorTest.cpp
)

target_link_libraries(chttl_tests
    PRIVATE
    gtest_main
    CHTL
    Util
)

include(GoogleTest)
gtest_discover_tests(chttl_tests)
