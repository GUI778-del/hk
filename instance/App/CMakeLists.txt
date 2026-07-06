# App 主程序 - 链接所有 DLL

add_executable(App
    main.c
)

target_include_directories(App PRIVATE ${CMAKE_SOURCE_DIR}/App)

# 链接三个 DLL
target_link_libraries(App PRIVATE Preprocess Index Search)
