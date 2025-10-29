add_library(StringUtil STATIC StringUtil.cpp)
target_include_directories(StringUtil PUBLIC ${CMAKE_SOURCE_DIR})

add_library(FileUtil STATIC FileUtil.cpp)
target_include_directories(FileUtil PUBLIC ${CMAKE_SOURCE_DIR})
