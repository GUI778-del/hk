/* =========================================================
 * Preprocess DLL - 预处理模块实现
 * ========================================================= */

#define MODULE_EXPORT
#include "preprocess.h"

/* ---------- read_file_to_string ---------- */
MODULE_API char* read_file_to_string(const char* path)
{
    FILE* fp = fopen(path, "r");
    if (fp == NULL) {
        printf("错误：无法打开文件 %s\n", path);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    rewind(fp);

    if (fsize <= 0) {
        printf("错误：文件 %s 为空\n", path);
        fclose(fp);
        return NULL;
    }

    char* content = (char*)malloc(fsize + 1);
    if (content == NULL) {
        printf("错误：内存分配失败\n");
        fclose(fp);
        return NULL;
    }

    size_t bytes_read = fread(content, 1, fsize, fp);
    content[bytes_read] = '\0';

    fclose(fp);
    return content;
}

/* ---------- tokenize ---------- */
MODULE_API int tokenize(const char* text, char*** out_words)
{
    if (text == NULL || text[0] == '\0') {
        *out_words = NULL;
        return 0;
    }

    int capacity = 128;
    int count = 0;
    char** words = (char**)malloc(capacity * sizeof(char*));
    if (words == NULL) {
        *out_words = NULL;
        return 0;
    }

    int i = 0;
    while (text[i] != '\0') {
        /* 跳过非字母数字 */
        while (text[i] != '\0' && !isalnum((unsigned char)text[i])) {
            i++;
        }
        if (text[i] == '\0') break;

        /* 收集单词 */
        int start = i;
        while (text[i] != '\0' && isalnum((unsigned char)text[i])) {
            i++;
        }
        int word_len = i - start;
        if (word_len >= MAX_WORD_LEN) word_len = MAX_WORD_LEN - 1;

        char* word = (char*)malloc((word_len + 1) * sizeof(char));
        if (word == NULL) continue;

        for (int j = 0; j < word_len; j++) {
            word[j] = tolower((unsigned char)text[start + j]);
        }
        word[word_len] = '\0';

        if (count >= capacity) {
            capacity *= 2;
            char** temp = (char**)realloc(words, capacity * sizeof(char*));
            if (temp == NULL) { free(word); break; }
            words = temp;
        }
        words[count++] = word;
    }

    *out_words = words;
    return count;
}

/* ---------- free_words ---------- */
MODULE_API void free_words(char** words, int count)
{
    if (words == NULL) return;
    for (int i = 0; i < count; i++) {
        if (words[i] != NULL) free(words[i]);
    }
    free(words);
}
