/* =========================================================
 * common.h - 全局通用定义（全队统一遵守的数据结构）
 * ========================================================= */

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* ---------- DLL 导出/导入宏（老师要求模块化 DLL） ---------- */
#ifdef _WIN32
  #ifdef MODULE_EXPORT
    #define MODULE_API __declspec(dllexport)
  #else
    #define MODULE_API __declspec(dllimport)
  #endif
#else
  /* Linux / WSL 下共享库默认导出所有符号 */
  #define MODULE_API
#endif

/* ---------- 常量 ---------- */
#define MAX_WORD_LEN  64
#define MAX_PATH_LEN  256
#define MAX_DOCS      100
#define MAX_LINE_LEN  1024

/* ---------- 1. 文档信息 ---------- */
typedef struct {
    int  doc_id;
    char path[MAX_PATH_LEN];
    int  total_words;   /* 文档总词数（用于评分归一化） */
} Document;

/* ---------- 2. 倒排索引节点（链表） ---------- */
typedef struct PostingNode {
    int  doc_id;
    int  freq;
    struct PostingNode* next;
} PostingNode;

/* ---------- 3. 词典节点（链表） ---------- */
typedef struct DictNode {
    char word[MAX_WORD_LEN];
    PostingNode* postings;
    struct DictNode* next;
} DictNode;

/* ---------- 4. 搜索结果 ---------- */
typedef struct {
    int  doc_id;
    int  score;
} SearchResult;

#endif /* COMMON_H */
