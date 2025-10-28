cmake_minimum_required(VERSION 3.16)
project(CHTL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include FetchContent for downloading dependencies
include(FetchContent)

# Include subdirectories for the main components
add_subdirectory(Util)
add_subdirectory(SharedCore)
add_subdirectory(CHTL)

# CHTL Executable
add_executable(chtl CHTL/main.cpp)
target_link_libraries(chtl PRIVATE CHTLLexer CHTLParser CHTLGenerator Util)


# Enable testing
enable_testing()

# Add test subdirectory
add_subdirectory(Test)
