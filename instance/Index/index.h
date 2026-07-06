/* =========================================================
 * Index DLL - 倒排索引模块（C同学负责）
 * ========================================================= */

#ifndef INDEX_H
#define INDEX_H

#define MODULE_EXPORT
#include "../App/common.h"

/* 初始化词典 */
MODULE_API void init_index(void);

/* 插入一个词到索引 */
MODULE_API void insert_word(const char* word, int doc_id);

/* 查找某个词的文档链表 */
MODULE_API PostingNode* find_word(const char* word);

/* 打印整个索引 */
MODULE_API void print_index(void);

/* 释放索引 */
MODULE_API void free_index(void);

#endif
