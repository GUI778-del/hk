# 预处理模块 DLL
# 编译为共享库（Linux .so / Windows .dll）

add_library(Preprocess SHARED
    preprocess.c
)

target_include_directories(Preprocess PUBLIC ${CMAKE_SOURCE_DIR}/App)
