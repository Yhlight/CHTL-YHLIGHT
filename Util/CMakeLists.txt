add_library(Util STATIC
    StringUtil.cpp
    FileUtil.cpp
)

target_include_directories(Util PUBLIC ${CMAKE_SOURCE_DIR})
