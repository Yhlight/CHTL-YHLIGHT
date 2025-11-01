# Create a library with the core compiler components
add_library(libchtl STATIC
    CHTL/CHTLLexer/Lexer.cpp
)

# Publicly expose the include directories of the library
target_include_directories(libchtl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Create the main executable
add_executable(chtl main.cpp)

# Link the executable against the library
target_link_libraries(chtl PRIVATE libchtl)
