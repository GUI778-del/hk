# 搜索模块 DLL
# 注意：Search 要调用 Index 的 find_word，所以需要链接 Index

add_library(Search SHARED
    search.c
)

target_include_directories(Search
    PUBLIC ${CMAKE_SOURCE_DIR}/App
    PRIVATE ${CMAKE_SOURCE_DIR}/Index
)

target_link_libraries(Search PRIVATE Index)
