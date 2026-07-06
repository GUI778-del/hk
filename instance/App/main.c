/* =========================================================
 * main.c - 程序入口
 *
 * 用法：
 *   ./App 文件1 文件2 文件3 ...     ← 用户指定一批文件
 *   ./App ./mydir/                  ← 扫描目录下所有 .txt
 *
 * 流程：
 *   1. 用户传入文件路径 → 加载文档
 *   2. 预处理(B) → 建索引(C)
 *   3. 交互搜索(含短语检索与benchmark) (D)
 * ========================================================= */

#include "common.h"
#include "../Preprocess/preprocess.h"
#include "../Index/index.h"
#include "../Search/search.h"

#include <unistd.h>
#include <sys/stat.h>

static Document docs[MAX_DOCS];
static int      doc_count = 0;

static void add_document(int doc_id, const char* path)
{
    docs[doc_id].doc_id = doc_id;
    strncpy(docs[doc_id].path, path, MAX_PATH_LEN - 1);
    docs[doc_id].path[MAX_PATH_LEN - 1] = '\0';
}

/* ---------- 判断路径是否为目录 ---------- */
static int is_directory(const char* path)
{
    struct stat st;
    if (stat(path, &st) != 0) return 0;
    return S_ISDIR(st.st_mode);
}

/* ---------- 检查文件是否存在 ---------- */
static int file_exists(const char* path)
{
    FILE* fp = fopen(path, "r");
    if (fp == NULL) return 0;
    fclose(fp);
    return 1;
}

/* ---------- 加载用户指定的文件 ---------- */
static int load_user_files(int argc, char* argv[])
{
    int id = 0;

    for (int i = 0; i < argc; i++) {
        const char* filepath = argv[i];

        if (id >= MAX_DOCS) {
            printf("警告：文件数量超过上限 %d，已忽略多余文件\n", MAX_DOCS);
            break;
        }

        /* 如果是目录，扫描该目录下的所有 .txt 文件 */
        if (is_directory(filepath)) {
            printf("  扫描目录: %s\n", filepath);
            char cmd[MAX_PATH_LEN + 50];
            snprintf(cmd, sizeof(cmd),
                     "find \"%s\" -maxdepth 1 -name '*.txt' 2>/dev/null",
                     filepath);
            FILE* fp = popen(cmd, "r");
            if (fp == NULL) continue;

            char line[MAX_PATH_LEN];
            while (fgets(line, sizeof(line), fp) != NULL) {
                size_t len = strlen(line);
                while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
                    line[--len] = '\0';
                if (len == 0) continue;
                if (line[0] == '\0') continue;
                /* 确认文件真实存在 */
                if (access(line, F_OK) != 0) continue;
                if (id >= MAX_DOCS) break;

                add_document(id, line);
                id++;
            }
            pclose(fp);
        }
        /* 否则直接作为文件路径 */
        else {
            if (!file_exists(filepath)) {
                printf("  警告：文件不存在，已跳过: %s\n", filepath);
                continue;
            }
            add_document(id, filepath);
            id++;
        }
    }

    doc_count = id;
    return doc_count;
}

/* ---------- 打印用法 ---------- */
static void print_usage(void)
{
    printf("========================================\n");
    printf("  Mini Text Search Engine\n");
    printf("  轻量文本检索与关键词搜索工具\n");
    printf("  纯C + DLL模块化  |  结构体 + 链表\n");
    printf("========================================\n\n");
    printf("用法:\n");
    printf("  %s 文件1 文件2 ...    指定一批文件\n", "App");
    printf("  %s ./目录/            扫描目录下所有 .txt 文件\n", "App");
    printf("  %s 文件1 ./目录/ 文件2 混合模式\n\n", "App");
    printf("示例:\n");
    printf("  App doc1.txt doc2.txt doc3.txt\n");
    printf("  App ../tests/\n");
    printf("  App /mnt/d/我的文档/文章1.txt 文章2.txt\n\n");
}

/* ---------- 构建索引 ---------- */
static void build_index(void)
{
    printf("\n正在构建索引...\n");
    for (int i = 0; i < doc_count; i++) {
        printf("  处理文档 %d: %s\n", docs[i].doc_id + 1, docs[i].path);

        char* content = read_file_to_string(docs[i].path);
        if (content == NULL) continue;

        char** words = NULL;
        int word_count = tokenize(content, &words);
        if (word_count <= 0) { free(content); continue; }

        for (int w = 0; w < word_count; w++) {
            insert_word(words[w], i);
        }
        printf("    提取了 %d 个单词\n", word_count);
        docs[i].total_words = word_count;

        free_words(words, word_count);
        free(content);
    }
    printf("索引构建完成！共 %d 个文档\n", doc_count);
}

/* ---------- 搜索帮助 ---------- */
static void print_search_help(void)
{
    printf("\n===== 搜索命令 =====\n");
    printf("  <关键词>          - 普通搜索\n");
    printf("  phrase <词1 词2>  - 短语检索（精确匹配）\n");
    printf("  bench <词1 词2>   - 批量测试（重复5次算平均时间）\n");
    printf("  help              - 显示此帮助\n");
    printf("  quit / exit       - 退出\n");
    printf("====================\n\n");
}

/* ---------- 交互式搜索 ---------- */
static void search_loop(void)
{
    char input[MAX_LINE_LEN];
    print_search_help();

    while (1) {
        printf("搜索 > ");
        if (fgets(input, sizeof(input), stdin) == NULL) break;

        size_t len = strlen(input);
        while (len > 0 && (input[len-1] == '\n' || input[len-1] == '\r'))
            input[--len] = '\0';

        if (strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0) {
            printf("再见！\n");
            break;
        }
        if (strcmp(input, "help") == 0) {
            print_search_help();
            continue;
        }
        if (strlen(input) == 0) continue;

        /* 判断命令 */
        int use_phrase = 0;
        int bench_mode = 0;
        char* search_text = input;

        if (strncmp(input, "phrase ", 7) == 0) {
            use_phrase = 1;
            search_text = input + 7;
            printf("短语检索模式\n");
        } else if (strncmp(input, "bench ", 6) == 0) {
            bench_mode = 1;
            search_text = input + 6;
        }

        /* 解析查询 */
        char** keywords = NULL;
        int num_keys = parse_query(search_text, &keywords);
        if (num_keys <= 0) {
            printf("请输入有效关键词。\n");
            continue;
        }

        /* Benchmark 模式 */
        if (bench_mode) {
            int num_trials = 5;
            double total_ms = 0.0;
            printf("Benchmark 模式：运行 %d 次...\n", num_trials);

            for (int t = 0; t < num_trials; t++) {
                clock_t start = clock();
                int rc = 0;
                SearchResult* r = search(keywords, num_keys, docs,
                                         doc_count, 5, &rc, 0);
                clock_t end = clock();
                total_ms += (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
                if (r != NULL) free(r);
            }

            double avg = total_ms / num_trials;
            printf("\n===== Benchmark 结果 =====\n");
            printf("  查询: \"%s\"\n", search_text);
            printf("  文档数: %d\n", doc_count);
            printf("  运行 %d 次平均响应时间: %.3f ms\n", num_trials, avg);
            printf("  总耗时: %.3f ms\n", total_ms);
            printf("===========================\n");

            free_keywords(keywords, num_keys);
            continue;
        }

        /* 普通搜索 / 短语检索 */
        clock_t start = clock();
        int result_count = 0;
        SearchResult* results = search(keywords, num_keys, docs, doc_count, 5,
                                       &result_count, use_phrase);
        clock_t end = clock();
        double time_ms = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

        printf("搜索 \"%s\" (%.3f ms):\n", search_text, time_ms);
        print_results(results, result_count, docs, keywords, num_keys);

        if (results != NULL) free(results);
        free_keywords(keywords, num_keys);
    }
}

/* ---------- main ---------- */
int main(int argc, char* argv[])
{
    /* 没有传入文件 → 显示用法 */
    if (argc < 2) {
        print_usage();
        return 1;
    }

    init_index();

    /* 加载用户传入的文件 */
    load_user_files(argc - 1, argv + 1);

    if (doc_count <= 0) {
        printf("错误：没有找到任何可读取的文档！\n");
        return 1;
    }
    printf("共加载 %d 个文档\n", doc_count);

    /* 构建索引 */
    build_index();

    /* 调试：打印索引概览（只看前10个词） */
    printf("\n索引构建完成，进入搜索模式。\n");

    /* 搜索 */
    search_loop();

    /* 释放内存 */
    free_index();
    printf("内存已释放，程序退出。\n");
    return 0;
}
