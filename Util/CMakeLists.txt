add_library(StringUtil STATIC StringUtil.cpp)
add_library(FileUtil STATIC File.cpp)

target_include_directories(StringUtil PUBLIC ${CMAKE_SOURCE_DIR})
target_include_directories(FileUtil PUBLIC ${CMAKE_SOURCE_DIR})
