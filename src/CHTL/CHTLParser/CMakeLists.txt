add_library(CHTLParser STATIC Parser.cpp)

target_include_directories(CHTLParser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(CHTLParser PUBLIC CHTLLexer CHTLNode)
