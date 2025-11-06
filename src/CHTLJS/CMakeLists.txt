add_library(libchtljs STATIC)

target_sources(libchtljs
    PRIVATE
    CHTLJSLexer/Lexer.cpp
    CHTLJSParser/Parser.cpp
    CHTLJSNode/CallExprNode.cpp
    CHTLJSNode/MemberAccessExprNode.cpp
    CHTLJSNode/SelectorExprNode.cpp
)

target_include_directories(libchtljs
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/src/SharedCore
)

add_subdirectory(CHTLJSGenerator)
target_link_libraries(libchtljs PUBLIC libchtljsgenerator)
