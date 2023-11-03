#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

typedef int TypeElem;
const size_t Poison = -22;
#define DEBUGONL

struct List {
    TypeElem *data;
    size_t *next;
    size_t *prev;
    size_t head;
    size_t tail;
    size_t size;
    size_t capacity;
    size_t free;
    size_t last_free;
    bool is_ctor;
    bool is_dtor;
};

enum {
    null_enter = -1,
    ok = 0,
    null_malloc = 1,
    null_data = 2,
    null_next = 3,
    null_prev = 4,
    just_ctor = 5,
    just_dtor = 6,
    dont_ctor = 7,
    size_capt = 8,
    impos_val_next = 9,
    impos_val_prev = 10,
    impos_val_free = 11,
    impos_val_last_free = 12,
    impos_enter_addres = 13,
    just_free_elem = 14,
};

int list_ctor(struct List *list, size_t beg_capacity);
int list_dtor(struct List *list);
int list_ver(const struct List *list);
size_t list_top(const struct List *list);
size_t list_end(const struct List *list);
size_t list_next_elem(const struct List *list, const size_t position);
size_t list_prev_elem(const struct List *list, const size_t position);
int list_push_front(struct List *list, const TypeElem new_elem);
int list_push_back(struct List *list, const TypeElem new_elem);
int list_push_brfore(struct List *list, const size_t addres_ref_elem, const TypeElem new_elem);
int list_push_after(struct List *list, const size_t addres_ref_elem, const TypeElem new_elem);
int list_remove(struct List *list, const size_t addres_rem_elem);
size_t list_search_by_value(struct List *list, const TypeElem value);
size_t list_search_by_number(struct List *list, const size_t num);
int list_destroy_humanity(struct List *list);
int conditional_realloc_increase(struct List *list);
int list_realloc(struct List *list, const size_t new_capacity);
int list_dump_html(struct List *list, FILE *dump_file_html, const int line, const char *func, const char *file);
int list_dump_file(const struct List *list, FILE *dump_file, const int line, const char *func, const char *file);
int list_dump_scheme(const struct List *list, const char *graph_file_name, const char *list_name);

#define LIST_DUMP(list) list_dump(list, __LINE__, __func__, __FILE__)
#define LIST_DUMP_HTML(list, dump_file_html) list_dump_html(list, dump_file_html, __LINE__, __func__, __FILE__);
#define LIST_DUMP_SCHEME(list, graph_file_name) list_dump_scheme(list, graph_file_name, #list);

#ifdef DEBUGONL
void decrypt_errors(const int error, const char *name_called_func, const int line, const char *func, const char *file, FILE* err_file);
#define DECRYPT_ERRORS(err, err_file) \
    decrypt_errors(err, #err, __LINE__, __func__, __FILE__, err_file)

#else 
#define DECRYPT_ERRORS(err, err_file) err; 

#endif 

#endif //LIST_H_INCLUDED