# Download Google Test
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG         main
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Add a test executable
add_executable(runTests)

# Link the test executable against Google Test and other libraries
target_link_libraries(runTests PRIVATE GTest::gtest_main StringUtil CHTLLexer CHTLParser CHTLGenerator)

# Function to add a new test file
function(add_test_file test_name file_path)
    target_sources(runTests PRIVATE ${file_path})
endfunction()

# Add test files
add_test_file(StringUtilTest ${CMAKE_SOURCE_DIR}/Test/Util/StringUtilTest.cpp)
add_test_file(LexerTest ${CMAKE_SOURCE_DIR}/Test/CHTL/LexerTest.cpp)
add_test_file(ParserTest ${CMAKE_SOURCE_DIR}/Test/CHTL/ParserTest.cpp)
add_test_file(GeneratorTest ${CMAKE_SOURCE_DIR}/Test/CHTL/GeneratorTest.cpp)
add_test_file(ImportTest ${CMAKE_SOURCE_DIR}/Test/ImportTest.cpp)
add_test_file(StyleBlockTest ${CMAKE_SOURCE_DIR}/Test/CHTL/StyleBlockTest.cpp)

# Include directories for headers
target_include_directories(runTests PRIVATE ${CMAKE_SOURCE_DIR})

# Discover and run tests
include(GoogleTest)
gtest_discover_tests(runTests)
