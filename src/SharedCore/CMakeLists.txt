add_library(libsharedcore STATIC)

target_sources(libsharedcore
    PRIVATE
    dummy.cpp
)

target_include_directories(libsharedcore
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
