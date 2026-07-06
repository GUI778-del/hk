/* =========================================================
 * Index DLL - 倒排索引模块实现
 * ========================================================= */

#define MODULE_EXPORT
#include "index.h"
#include <string.h>

static DictNode* dict_head = NULL;

MODULE_API void init_index(void)
{
    dict_head = NULL;
}

MODULE_API void insert_word(const char* word, int doc_id)
{
    if (word == NULL || word[0] == '\0') return;

    DictNode* cur = dict_head;
    while (cur != NULL) {
        if (strcmp(cur->word, word) == 0) {
            PostingNode* p = cur->postings;
            while (p != NULL) {
                if (p->doc_id == doc_id) {
                    p->freq++;
                    return;
                }
                p = p->next;
            }
            PostingNode* new_p = (PostingNode*)malloc(sizeof(PostingNode));
            if (new_p == NULL) return;
            new_p->doc_id = doc_id;
            new_p->freq   = 1;
            new_p->next   = cur->postings;
            cur->postings = new_p;
            return;
        }
        cur = cur->next;
    }

    DictNode* new_d = (DictNode*)malloc(sizeof(DictNode));
    if (new_d == NULL) return;
    strncpy(new_d->word, word, MAX_WORD_LEN - 1);
    new_d->word[MAX_WORD_LEN - 1] = '\0';

    PostingNode* new_p = (PostingNode*)malloc(sizeof(PostingNode));
    if (new_p == NULL) { free(new_d); return; }
    new_p->doc_id = doc_id;
    new_p->freq   = 1;
    new_p->next   = NULL;
    new_d->postings = new_p;

    new_d->next = dict_head;
    dict_head = new_d;
}

MODULE_API PostingNode* find_word(const char* word)
{
    if (word == NULL) return NULL;
    DictNode* cur = dict_head;
    while (cur != NULL) {
        if (strcmp(cur->word, word) == 0) {
            return cur->postings;
        }
        cur = cur->next;
    }
    return NULL;
}

MODULE_API void print_index(void)
{
    if (dict_head == NULL) {
        printf("（索引为空）\n");
        return;
    }
    printf("\n===== 索引内容 =====\n");
    DictNode* d = dict_head;
    while (d != NULL) {
        printf("词: %s\n", d->word);
        PostingNode* p = d->postings;
        while (p != NULL) {
            printf("   └─ 文档 %d, 出现 %d 次\n", p->doc_id + 1, p->freq);
            p = p->next;
        }
        d = d->next;
    }
    printf("====================\n");
}

MODULE_API void free_index(void)
{
    DictNode* d = dict_head;
    while (d != NULL) {
        PostingNode* p = d->postings;
        while (p != NULL) {
            PostingNode* next_p = p->next;
            free(p);
            p = next_p;
        }
        DictNode* next_d = d->next;
        free(d);
        d = next_d;
    }
    dict_head = NULL;
}
