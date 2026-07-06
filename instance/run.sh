#!/bin/bash
# 一键编译 + 运行（使用 tests/ 目录下的文件做演示）
cd "$(dirname "$0")"

mkdir -p build && cd build
cmake .. || exit 1
make || exit 1
cd ..

echo ""
echo "===== 编译成功！使用 tests/ 目录下的文件演示 ====="
LD_LIBRARY_PATH=./build/bin ./build/bin/App tests/
