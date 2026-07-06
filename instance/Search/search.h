/* =========================================================
 * Search DLL - 搜索模块（D同学负责）
 *
 * 功能：解析查询、搜索索引、评分排序、支持短语检索
 * ========================================================= */

#ifndef SEARCH_H
#define SEARCH_H

#define MODULE_EXPORT
#include "../App/common.h"

/* 按空格拆分查询字符串 */
MODULE_API int parse_query(const char* query, char*** out_keywords);

/*
 * 搜索：对每个关键词查索引，累加文档得分，排序取 Top-K
 * keywords   - 关键词数组
 * num_keys   - 关键词个数
 * documents  - 文档信息数组（含总词数，用于归一化评分）
 * num_docs   - 文档总数
 * top_k      - 返回前多少个
 * use_phrase - 是否启用短语检索
 */
MODULE_API SearchResult* search(char** keywords, int num_keys,
                                Document* documents, int num_docs,
                                int top_k, int* count,
                                int use_phrase);

/* 
 * 打印搜索结果（含每个关键词在各文档的出现次数）
 * keywords / num_keys - 用于显示关键词频次明细
 */
MODULE_API void print_results(SearchResult* results, int count,
                              Document* documents,
                              char** keywords, int num_keys);

/* 释放关键词数组 */
MODULE_API void free_keywords(char** keywords, int count);

#endif
