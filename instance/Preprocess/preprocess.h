/* =========================================================
 * Preprocess DLL - 文本预处理模块（B同学负责）
 * ========================================================= */

#ifndef PREPROCESS_H
#define PREPROCESS_H

/* 编译本 DLL 时导出符号，外部调用时导入 */
#define MODULE_EXPORT
#include "../App/common.h"

/* 读取整个 txt 文件到字符串 */
MODULE_API char* read_file_to_string(const char* path);

/* 将文本切词：转小写、去标点，按空白分割 */
MODULE_API int tokenize(const char* text, char*** out_words);

/* 释放单词数组 */
MODULE_API void free_words(char** words, int count);

#endif
