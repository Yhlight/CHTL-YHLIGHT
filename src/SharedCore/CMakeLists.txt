add_library(libsharedcore)

target_sources(libsharedcore
  PRIVATE
    Dummy.cpp
)

target_include_directories(libsharedcore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
