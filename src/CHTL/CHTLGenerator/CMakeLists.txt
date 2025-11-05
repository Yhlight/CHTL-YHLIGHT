add_library(CHTLGenerator STATIC Generator.cpp)

target_include_directories(CHTLGenerator PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(CHTLGenerator PUBLIC CHTLNode Util)
