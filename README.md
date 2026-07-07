============================================================
  Mini Text Search Engine
  轻量级文本检索与关键词搜索工具
  纯C + DLL模块化 | 数据结构课程设计
============================================================

【目录】
  一、项目简介
  二、技术栈
  三、项目结构
  四、快速开始
  五、使用说明
  六、模块详解
  七、运行示例
  八、开发指南
  九、团队成员分工
  十、附录


一、项目简介
============================================================

Mini Text Search Engine 是一个用纯C语言实现的轻量级搜索
引擎，支持以下功能：

  * 分词预处理 - 读取文本文件，去标点、转小写、按空白分割
  * 倒排索引   - 使用链表结构构建"词到文档"的映射关系
  * 关键词搜索 - 输入关键词，按评分排序返回Top-K结果
  * 短语检索   - 精确匹配，要求所有关键词同时出现在同一文档
  * Benchmark  - 内置性能测试，自动运行多次取平均响应时间
  * 命令行传参 - 支持传入文件路径或扫描目录

项目采用DLL模块化架构（Linux下为.so共享库），三个功能模块
独立编译，主程序通过链接调用。


二、技术栈
============================================================

  语言      C99
  构建系统  CMake >= 3.10
  编译器    GCC / MinGW
  模块化    共享库（Linux .so / Windows .dll）
  数据结构  单向链表（词典 + 倒排索引）
  运行环境  Linux / WSL / Windows（通过WSL或MinGW）


三、项目结构
============================================================

MiniSearch/
├── CMakeLists.txt              # 根构建文件
├── App/                        # 主程序模块（A负责）
│   ├── CMakeLists.txt
│   ├── common.h                # 全局数据结构
│   └── main.c                  # 程序入口、文件加载、交互搜索
├── Preprocess/                 # 预处理模块（B负责）
│   ├── CMakeLists.txt
│   ├── preprocess.h
│   └── preprocess.c            # 读文件、分词、释放
├── Index/                      # 倒排索引模块（C负责）
│   ├── CMakeLists.txt
│   ├── index.h
│   └── index.c                 # 插入、查找、打印、释放
├── Search/                     # 搜索模块（D负责）
│   ├── CMakeLists.txt
│   ├── search.h
│   └── search.c                # 查询解析、搜索排序、结果展示
├── input/                      # 演示文档目录
│   ├── test_1.txt
│   └── test_2.txt
├── tests/                      # 测试文档目录（6篇）
│   ├── test_1.txt ~ test_6.txt
├── run.sh                      # Linux/WSL一键编译运行
├── run.bat                     # Windows一键编译运行
├── MiniSearch.bat              # 快速运行（跳过编译）
├── MiniSearch___编译.bat       # 仅编译
└── build/                      # 编译输出目录（自动生成）
    └── bin/
        ├── App                 # 可执行文件
        ├── libPreprocess.so    # 预处理共享库
        ├── libIndex.so         # 索引共享库
        └── libSearch.so        # 搜索共享库


四、快速开始
============================================================

【环境要求】
  - CMake >= 3.10
  - GCC（支持C99标准）
  - （Windows下）WSL或MinGW

【编译】
  cd MiniSearch
  mkdir -p build && cd build
  cmake ..
  make
  cd ..

【运行】
  # 方式一：用input/目录下的文件演示
  LD_LIBRARY_PATH=./build/bin ./build/bin/App input/

  # 方式二：直接指定文件
  LD_LIBRARY_PATH=./build/bin ./build/bin/App input/test_1.txt input/test_2.txt

  # 方式三：混合模式
  LD_LIBRARY_PATH=./build/bin ./build/bin/App ./input/ tests/test_1.txt

【Windows下运行】
  双击 run.bat 即可一键编译+运行（演示文件用tests/目录）。
  也可将文件拖拽到run.bat图标上，自动转换为WSL路径后执行。


五、使用说明
============================================================

【命令行参数】
  用法:
    App 文件1 文件2 ...         指定一批文件
    App ./目录/                 扫描目录下所有.txt文件
    App 文件1 ./目录/ 文件2     混合模式

  示例:
    App doc1.txt doc2.txt doc3.txt
    App ../tests/
    App /mnt/d/我的文档/文章1.txt 文章2.txt

  如果不传任何参数，程序会显示使用帮助。

【交互式搜索命令】
  程序加载完文档后进入交互模式:

  命令                        说明                示例
  --------------------------- ------------------ -----------------
  关键词                      普通搜索            pi
  关键词1 关键词2 ...         多关键词组合搜索    pi monte carlo
  phrase 词1 词2 ...          短语检索(精确匹配)  phrase pi day
  bench 词1 词2 ...           Benchmark性能测试   bench pi random
  help                        显示帮助            help
  quit / exit                 退出程序            quit


六、模块详解
============================================================

6.1 全局数据结构（common.h）

所有模块共享的数据结构，定义在App/common.h中：

  // DLL导出/导入宏（Windows专用，Linux下为空）
  #ifdef _WIN32
    #ifdef MODULE_EXPORT
      #define MODULE_API __declspec(dllexport)
    #else
      #define MODULE_API __declspec(dllimport)
    #endif
  #else
    #define MODULE_API
  #endif

  #define MAX_WORD_LEN  64
  #define MAX_DOCS      100
  #define MAX_PATH_LEN  256
  #define MAX_LINE_LEN  1024

  // 文档信息
  typedef struct {
      int  doc_id;
      char path[MAX_PATH_LEN];
      int  total_words;       // 文档总词数（用于评分归一化）
  } Document;

  // 倒排索引 - 文档链表节点
  typedef struct PostingNode {
      int  doc_id;
      int  freq;
      struct PostingNode* next;
  } PostingNode;

  // 倒排索引 - 词典节点
  typedef struct DictNode {
      char word[MAX_WORD_LEN];
      PostingNode* postings;
      struct DictNode* next;
  } DictNode;

  // 搜索结果
  typedef struct {
      int  doc_id;
      int  score;
  } SearchResult;


6.2 App模块（main.c）

主程序入口，执行流程：
  1. 解析命令行参数，加载用户指定的文件或目录
  2. 对每个文档调用Preprocess模块进行预处理
  3. 将分词结果插入Index模块的倒排索引
  4. 进入交互搜索循环，调Search模块执行查询

文件加载支持三种模式：单个文件、目录扫描（find命令+
access校验）、混合模式。


6.3 Preprocess模块

三个导出函数：

  read_file_to_string(path)
    功能：读取整个文件到堆内存字符串
    返回值：char*（需调用者free）

  tokenize(text, &out_words)
    功能：分词：去标点、转小写、按空白分割
    返回值：单词个数

  free_words(words, count)
    功能：释放单词数组

分词规则：
  * 非字母数字字符（标点、空格、换行）作为分隔符
  * 所有字母转为小写
  * 单词过长（>=64字符）截断
  * 动态扩容（初始128，翻倍增长）


6.4 Index模块

五个导出函数：

  init_index()      初始化词典（置空）
  insert_word(w,d)  插入一个词到倒排索引
  find_word(word)   查找某词的文档链表
  print_index()     打印全部索引内容（调试用）
  free_index()      释放所有索引内存

数据结构：双层单向链表。
  * 外层：词典链表（DictNode），按插入顺序排列
  * 内层：文档链表（PostingNode），头插法

示意图：
  词典链表:
    [word:"pi" postings-->] -> [word:"the" postings-->] -> ...
         |                          |
         v                          v
    [doc_id:0, freq:2]         [doc_id:0, freq:5]
    [doc_id:1, freq:1]         [doc_id:1, freq:3]
    next-->NULL                next-->NULL


6.5 Search模块

四个导出函数：

  parse_query(query, &keywords)
    功能：按空格拆分查询字符串

  search(keywords, num_keys, docs, num_docs, top_k, &count, use_phrase)
    功能：核心搜索

  print_results(results, count, docs, keywords, num_keys)
    功能：打印搜索结果

  free_keywords(keywords, count)
    功能：释放关键词数组

搜索算法：
  1. 对每个关键词调find_word获取文档链表
  2. 累加归一化评分 -- freq * 10000 / total_words
  3. 短语模式下，要求所有关键词出现在同一文档
  4. 冒泡排序降序，取Top-K

Benchmark模式运行5次取平均响应时间。


七、运行示例
============================================================

示例1：普通搜索

  $ LD_LIBRARY_PATH=./build/bin ./build/bin/App input/
    扫描目录: input/
    处理文档 1: .../input/test_1.txt
      提取了 813 个单词
    处理文档 2: .../input/test_2.txt
      提取了 336 个单词
  索引构建完成！共 2 个文档

  搜索 > pi monte carlo
  搜索 "pi monte carlo" (0.018 ms):
  ====== 搜索结果 ======
  第 1 名: 文档 1 (input/test_1.txt) (总分: 155)
      关键词 "pi" 出现 9 次
      关键词 "monte" 出现 4 次
      关键词 "carlo" 出现 1 次
  ======================

示例2：短语检索

  搜索 > phrase pi day
  短语检索模式
  搜索 "pi day" (0.009 ms):
  ====== 搜索结果 ======
  第 1 名: 文档 1 (input/test_1.txt) (总分: 73)
      关键词 "pi" 出现 9 次
      关键词 "day" 出现 3 次
  ======================

示例3：Benchmark

  搜索 > bench elderly society
  Benchmark 模式：运行 5 次...
  ===== Benchmark 结果 =====
    查询: "elderly society"
    文档数: 2
    运行 5 次平均响应时间: 0.032 ms
    总耗时: 0.160 ms
  ===========================


八、开发指南
============================================================

【添加新模块】
  1. 在项目根目录创建新目录ModuleX/
  2. 添加ModuleX/CMakeLists.txt：
       add_library(ModuleX SHARED module_x.c)
       target_include_directories(ModuleX PUBLIC ${CMAKE_SOURCE_DIR}/App)
  3. 在根CMakeLists.txt添加add_subdirectory(ModuleX)
  4. 如需链接其他DLL，在target_link_libraries中添加

【DLL导出宏约定】
  每个模块的.h文件顶部：
    #define MODULE_EXPORT
    #include "../App/common.h"

  每个模块的.c文件顶部：
    #define MODULE_EXPORT
    #include "module.h"

【内存管理原则】
  * read_file_to_string返回的内存由调用者free
  * tokenize分配的单词数组由free_words释放
  * search返回的结果数组由调用者free
  * parse_query分配的关键词数组由free_keywords释放
  * 程序退出前调用free_index释放全部索引内存


九、团队成员分工
============================================================

  成员  模块        文件                      职责
  ----  -----------  ------------------------  ------------------------------
  A     App          main.c, common.h,         程序入口、文件加载、搜索循环
                     CMakeLists.txt            构建系统、运行脚本
  B     Preprocess   preprocess.c/h            文本读取、分词、去标点、转小写
  C     Index        index.c/h                 倒排索引数据结构、插入、查找
  D     Search       search.c/h                查询解析、评分排序、短语检索、
                                               Benchmark


十、附录
============================================================

【文件清单】
  文件                              说明
  --------------------------------- ----------------------------
  CMakeLists.txt                    根构建文件
  App/common.h                      全局数据结构定义
  App/main.c                        主程序入口
  App/CMakeLists.txt                App构建配置
  Preprocess/preprocess.h           预处理模块头文件
  Preprocess/preprocess.c           预处理模块实现
  Preprocess/CMakeLists.txt         Preprocess构建配置
  Index/index.h                     索引模块头文件
  Index/index.c                     索引模块实现
  Index/CMakeLists.txt              Index构建配置
  Search/search.h                   搜索模块头文件
  Search/search.c                   搜索模块实现
  Search/CMakeLists.txt             Search构建配置
  input/test_1.txt                  演示文档1（Pi主题）
  input/test_2.txt                  演示文档2（老龄化主题）
  tests/test_1.txt ~ test_6.txt     测试文档（6篇）
  run.sh                            Linux/WSL一键编译运行
  run.bat                           Windows一键编译运行
  MiniSearch.bat                    快速运行（跳过编译）
  MiniSearch___编译.bat             仅编译

【已知限制】
  * 不支持中文分词（isalnum对中文返回0）
  * 词典查找为线性O(n)，大规模数据建议改用哈希表
  * 排序用冒泡排序，可优化为快速排序
  * 评分算法基于词频密度，未包含TF-IDF等更复杂的权重

============================================================
  文件版本: v1.0
  最后更新: 2026-07-07
  课程: 数据结构课程设计
============================================================
