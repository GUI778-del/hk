/* =========================================================
 * Search DLL - 搜索模块实现
 *
 * 搜索流程：
 *   1. parse_query 拆分用户输入
 *   2. search 遍历关键词 → find_word → 累加 freq → 排序
 *   3. 加分项：短语检索 (use_phrase=1)
 *   4. print_results 打印（含每个关键词的频次明细）
 * ========================================================= */

#define MODULE_EXPORT
#include "search.h"
#include "../Index/index.h"   /* 调 C 的 find_word */

/* ---------- parse_query ---------- */
MODULE_API int parse_query(const char* query, char*** out_keywords)
{
    if (query == NULL || query[0] == '\0') {
        *out_keywords = NULL;
        return 0;
    }

    char* copy = (char*)malloc((strlen(query) + 1) * sizeof(char));
    if (copy == NULL) { *out_keywords = NULL; return 0; }
    strcpy(copy, query);

    int capacity = 16, count = 0;
    char** keywords = (char**)malloc(capacity * sizeof(char*));
    if (keywords == NULL) { free(copy); *out_keywords = NULL; return 0; }

    char* token = strtok(copy, " ");
    while (token != NULL) {
        if (token[0] != '\0') {
            if (count >= capacity) {
                capacity *= 2;
                char** temp = (char**)realloc(keywords, capacity * sizeof(char*));
                if (temp == NULL) break;
                keywords = temp;
            }
            keywords[count] = (char*)malloc((strlen(token) + 1) * sizeof(char));
            if (keywords[count] != NULL) {
                strcpy(keywords[count], token);
                count++;
            }
        }
        token = strtok(NULL, " ");
    }

    free(copy);
    *out_keywords = keywords;
    return count;
}

/* ---------- free_keywords ---------- */
MODULE_API void free_keywords(char** keywords, int count)
{
    if (keywords == NULL) return;
    for (int i = 0; i < count; i++) {
        if (keywords[i] != NULL) free(keywords[i]);
    }
    free(keywords);
}

/* ---------- search (核心) ---------- */
MODULE_API SearchResult* search(char** keywords, int num_keys,
                                Document* documents, int num_docs,
                                int top_k, int* count,
                                int use_phrase)
{
    if (keywords == NULL || num_keys <= 0 || num_docs <= 0) {
        *count = 0;
        return NULL;
    }

    int* scores = (int*)calloc(num_docs, sizeof(int));
    if (scores == NULL) { *count = 0; return NULL; }

    /* 对每个关键词查索引 */
    for (int k = 0; k < num_keys; k++) {
        PostingNode* p = find_word(keywords[k]);
        while (p != NULL) {
            if (p->doc_id >= 0 && p->doc_id < num_docs) {
                /* 
                 * 归一化评分：词频 / 文档总词数 * 10000
                 * 这样短文档的关键词密度更高，得分就更高
                 * 乘 10000 是为了保留精度（整数运算）
                 */
                int total = documents[p->doc_id].total_words;
                if (total > 0) {
                    scores[p->doc_id] += p->freq * 10000 / total;
                }
            }
            p = p->next;
        }
    }

    /* 短语检索：只有所有关键词都在同一文档中才给分 */
    if (use_phrase && num_keys > 1) {
        for (int i = 0; i < num_docs; i++) {
            int hit_count = 0;
            for (int k = 0; k < num_keys; k++) {
                PostingNode* p = find_word(keywords[k]);
                int found = 0;
                while (p != NULL) {
                    if (p->doc_id == i) { found = 1; break; }
                    p = p->next;
                }
                if (found) hit_count++;
            }
            if (hit_count < num_keys) {
                scores[i] = 0;
            }
        }
    }

    /* 收集有得分的文档 */
    SearchResult* all = (SearchResult*)malloc(num_docs * sizeof(SearchResult));
    if (all == NULL) { free(scores); *count = 0; return NULL; }

    int total = 0;
    for (int i = 0; i < num_docs; i++) {
        if (scores[i] > 0) {
            all[total].doc_id = i;
            all[total].score  = scores[i];
            total++;
        }
    }

    /* 冒泡排序降序 */
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - 1 - i; j++) {
            if (all[j].score < all[j + 1].score) {
                SearchResult tmp = all[j];
                all[j] = all[j + 1];
                all[j + 1] = tmp;
            }
        }
    }

    /* 取 top_k */
    int result_count = (total < top_k) ? total : top_k;
    SearchResult* results = (SearchResult*)malloc(
                                result_count * sizeof(SearchResult));
    if (results == NULL) { free(all); free(scores); *count = 0; return NULL; }

    for (int i = 0; i < result_count; i++) {
        results[i] = all[i];
    }

    free(all);
    free(scores);
    *count = result_count;
    return results;
}

/* ---------- 辅助：查某个关键词在某个文档中的出现次数 ---------- */
static int get_word_freq_in_doc(const char* word, int doc_id)
{
    PostingNode* p = find_word(word);
    while (p != NULL) {
        if (p->doc_id == doc_id) {
            return p->freq;
        }
        p = p->next;
    }
    return 0;
}

/* ---------- print_results （含关键词频次明细） ---------- */
MODULE_API void print_results(SearchResult* results, int count,
                              Document* documents,
                              char** keywords, int num_keys)
{
    if (results == NULL || count <= 0) {
        printf("没有找到匹配的结果。\n");
        return;
    }
    printf("\n====== 搜索结果 ======\n");
    for (int i = 0; i < count; i++) {
        int doc_id = results[i].doc_id;
        printf("第 %d 名: 文档 %d", i + 1, doc_id + 1);
        if (documents != NULL && doc_id >= 0) {
            printf(" (%s)", documents[doc_id].path);
        }
        printf(" (总分: %d)\n", results[i].score);

        /* 显示每个关键词在该文档中的具体出现次数 */
        if (keywords != NULL && num_keys > 0) {
            for (int k = 0; k < num_keys; k++) {
                int freq = get_word_freq_in_doc(keywords[k], doc_id);
                if (freq > 0) {
                    printf("    关键词 \"%s\" 出现 %d 次\n",
                           keywords[k], freq);
                }
            }
        }
    }
    printf("======================\n");
}
