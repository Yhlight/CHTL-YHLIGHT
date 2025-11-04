# Add GTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(run_tests)

target_sources(run_tests PRIVATE
    InitialTest.cpp
    CHTL/LexerTest.cpp
    CHTL/ParserTest.cpp
    CHTL/GeneratorTest.cpp
)

target_include_directories(run_tests PRIVATE
    ../src
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(run_tests PRIVATE
    libchtl
    libchtljs
    libsharedcore
    GTest::gtest
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(run_tests)
