add_library(CHTLJSGenerator
    Generator.cpp
)

target_include_directories(CHTLJSGenerator PUBLIC .)
target_link_libraries(CHTLJSGenerator PUBLIC CHTLJSNode)
