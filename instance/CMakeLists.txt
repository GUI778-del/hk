cmake_minimum_required(VERSION 3.10)
project(MiniSearch C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 所有 .so / .dll 生成到 build/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 进入子目录
add_subdirectory(Preprocess)
add_subdirectory(Index)
add_subdirectory(Search)
add_subdirectory(App)
