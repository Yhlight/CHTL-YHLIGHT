cmake_minimum_required(VERSION 3.10)
project(CHTL_Tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(runTests
    main.cpp
    LexerTest.cpp
)

target_link_libraries(runTests PRIVATE CHTLLexer)

# Since CHTLLexer is in a sibling directory, I need to tell CMake where to find it.
# The main CMakeLists.txt already handles the add_subdirectory, so linking should be fine.
