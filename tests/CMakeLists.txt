add_executable(run_tests)

target_sources(run_tests
    PRIVATE
    LexerTest.cpp
    ParserTest.cpp
    GeneratorTest.cpp
)

target_link_libraries(run_tests
    PRIVATE
    gtest_main
    libchtl
    libchtljs
    libsharedcore
)

target_include_directories(run_tests
    PRIVATE
    ../src
    ${CMAKE_CURRENT_SOURCE_DIR}
)

include(GoogleTest)
gtest_discover_tests(run_tests)
