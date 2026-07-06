# 索引模块 DLL

add_library(Index SHARED
    index.c
)

target_include_directories(Index PUBLIC ${CMAKE_SOURCE_DIR}/App)
