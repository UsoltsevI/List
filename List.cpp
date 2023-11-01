#include "List.h"

int list_ctor(struct List *list, size_t beg_capacity) {
    if (list == NULL)
        return null_enter;
    
    if (list->is_dtor == 1)
        return just_dtor;

    if (list->is_ctor == 1)
        return just_ctor;

    list->data = (TypeElem *) calloc(beg_capacity + 2, sizeof(TypeElem));
    list->next = (size_t *) calloc(beg_capacity + 2, sizeof(size_t));
    list->prev = (size_t *) calloc(beg_capacity + 2, sizeof(size_t));

    if (list->data == NULL)
        return null_data;
    
    if (list->next == NULL)
        return null_next;
    
    if (list->prev == NULL)
        return null_prev;

    list->capacity = beg_capacity;
    list->is_ctor = 1;
    list->is_dtor = 0;
    list->free = 2;
    list->size = 0;
    list->head = 0;
    list->tail = 1;
    list->last_free = beg_capacity + 1;
    list->next[list->head] = list->tail;
    list->next[list->tail] = list->head;
    list->prev[list->head] = list->tail;
    list->prev[list->tail] = list->head;

    for (size_t i = 2; i < list->capacity + 1; i++) {
        list->next[i] = i + 1;
        list->prev[i] = Poison;
        list->data[i] = (TypeElem) Poison;
    }

    list->data[list->capacity + 1] = (TypeElem) Poison;
    list->next[list->capacity + 1] = list->last_free;
    list->prev[list->capacity + 1] = Poison;

    return ok;
}

int list_dtor(struct List *list) {
    int check = list_ver(list);
    if (check) return check;

    free(list->data);
    free(list->next);
    free(list->prev);

    list->data = NULL;
    list->next = NULL;
    list->prev = NULL;

    list->head = -1;
    list->tail = -1;
    list->size = -1;
    list->capacity = -1;
    list->free = -1;
    list->last_free = -1;
    list->is_dtor = 1;

    return ok;
}

int list_ver(const struct List *list) {
    if (list == NULL)
        return null_enter;
    
    if (list->is_dtor == 1)
        return just_dtor;

    if (list->is_ctor == 0)
        return dont_ctor;
    
    if (list->data == NULL)
        return null_data;
    
    if (list->next == NULL)
        return null_next;
    
    if (list->prev == NULL)
        return null_prev;
    
    if (list->size > list->capacity)
        return size_capt;

    for (size_t i = 0; i < list->capacity + 2; i++) {
        if (list->next[i] >= list->capacity + 2) 
            return impos_val_next;
        
        if ((list->prev[i] >= list->capacity + 2) && (list->prev[i] != Poison))
            return impos_val_prev;
    }
    
    if ((list->free >= list->capacity + 2) || (list->free < 2)) 
        return impos_val_free;
    
    if ((list->last_free >= list->capacity + 2) || (list->free < 2))
        return impos_val_last_free;

    return ok;
}

size_t list_top(const struct List *list) {
    int check = list_ver(list);
    if (check) return -1;

    if (list->size == 0) return -1;
    
    return list->next[list->head] - 2;
}

size_t list_end(const struct List *list) {
    int check = list_ver(list);
    if (check) return -1;

    if (list->size == 0) return -1;
    
    return list->prev[list->tail] - 2;
}

size_t list_next_elem(const struct List *list, const size_t position) {
    int check = list_ver(list);
    if (check) return -1;
    
    if (position >= list->capacity) return -1;

    return list->next[position + 2];
}

size_t list_prev_elem(const struct List *list, const size_t position) {
    int check = list_ver(list);
    if (check) return -1;

    if (position >= list->capacity) return -1;
    
    return list->prev[position + 2];
}

int list_push_front(struct List *list, const TypeElem new_elem) {
    int check = list_ver(list);
    if (check) return check;

    conditional_realloc_increase(list);

    size_t addres_new_elem = list->free;
    list->free = list->next[list->free];
    list->data[addres_new_elem] = new_elem;
    list->next[addres_new_elem] = list->next[list->head];
    list->prev[addres_new_elem] = list->head;
    list->next[list->head] = addres_new_elem;
    list->size++;

    if (list->size == 1)
        list->prev[list->tail] = addres_new_elem;

    return ok;
}

int list_push_back(struct List *list, const TypeElem new_elem) {
    int check = list_ver(list);
    if (check) return check;

    conditional_realloc_increase(list);

    size_t addres_new_elem = list->free;
    list->free = list->next[list->free]; 
    list->data[addres_new_elem] = new_elem;
    list->next[addres_new_elem] = list->tail;
    list->prev[addres_new_elem] = list->prev[list->tail];
    list->next[list->prev[list->tail]] = addres_new_elem;
    list->prev[list->tail] = addres_new_elem;
    list->size++;

    if (list->size == 1)
        list->prev[list->tail] = addres_new_elem;

    return ok;
}

int list_push_brfore(struct List *list, const size_t addres_ref_elem, const TypeElem new_elem) {
    int check = list_ver(list);
    if (check) return check;

    conditional_realloc_increase(list);

    if (addres_ref_elem >= list->capacity)
        return impos_enter_addres;

    size_t addres_new_elem = list->free;
    list->free = list->next[list->free];
    list->data[addres_new_elem] = new_elem;
    list->next[addres_new_elem] = addres_ref_elem + 2;
    list->prev[addres_new_elem] = list->prev[addres_ref_elem + 2];
    list->next[list->prev[addres_ref_elem + 2]] = addres_new_elem;
    list->prev[addres_ref_elem + 2] = addres_new_elem;
    list->size++;

    if (list->size == 1)
        list->prev[list->tail] = addres_new_elem;

    return ok;
}

int list_push_after(struct List *list, const size_t addres_ref_elem, const TypeElem new_elem) {
    int check = list_ver(list);
    if (check) return check;

    conditional_realloc_increase(list);

    if (addres_ref_elem >= list->capacity)
        return impos_enter_addres;

    size_t addres_new_elem = list->free;
    list->free = list->next[list->free]; 
    list->data[addres_new_elem] = new_elem;
    list->next[addres_new_elem] = list->next[addres_ref_elem + 2];
    list->prev[addres_new_elem] = addres_ref_elem + 2;
    list->prev[list->next[addres_ref_elem + 2]] = addres_new_elem;
    list->next[addres_ref_elem + 2] = addres_new_elem;
    list->size++;

    if (list->size == 1)
        list->prev[list->tail] = addres_new_elem;

    return ok;
}

int list_remove(struct List *list, const size_t addres_rem_elem) {
    int check = list_ver(list);
    if (check) return check;

    if (addres_rem_elem >= list->capacity)
        return impos_enter_addres;

    size_t addres_rem = addres_rem_elem + 2;

    if (list->prev[addres_rem] == Poison)
        return just_free_elem;

    list->prev[list->next[addres_rem]] = list->prev[addres_rem];
    list->next[list->prev[addres_rem]] = list->next[addres_rem];
    list->next[list->last_free] = addres_rem;
    list->last_free = addres_rem;
    list->next[addres_rem] = list->last_free;
    list->prev[addres_rem] = Poison;
    list->data[addres_rem] = (TypeElem) Poison;

    list->size--;

    return ok;
}

size_t list_search_by_value(struct List *list, const TypeElem value) {
    int check = list_ver(list);
    if (check) return -1;

    for (size_t i = 2; i < list->capacity + 2; i++)
        if (list->data[i] == value)
            return i - 2;

    return -1;
}

size_t list_search_by_number(struct List *list, const size_t num) {
    int check = list_ver(list);
    if (check) return -1;

    if (num > list->size) return -1;

    size_t i = 2;

    while (i < num + 2) {
        if (list->next[i] == list->tail)
            return -1;
        
        i = list->next[i];
    }

    return i - 2;
}

int list_destroy_humanity(struct List *list) {
    int check = list_ver(list);
    if (check) return check;

    list->next[list->head] = list->tail;
    list->prev[list->tail] = list->head;

    return list_realloc(list, 0);
}

int conditional_realloc_increase(struct List *list) {
    if (list->size + 2 >= list->capacity) {
        if (list->capacity == 0)
            list->capacity = 2;
        
        return list_realloc(list, list->capacity * 2);
    }

    return ok;
}

int list_realloc(struct List *list, const size_t new_capacity) {
    int check = list_ver(list);
    if (check) return check;

    list->data = (TypeElem *) realloc(list->data, (new_capacity + 2) * sizeof(TypeElem));
    list->next = (size_t *) realloc(list->next, (new_capacity + 2) * sizeof(size_t));
    list->prev = (size_t *) realloc(list->prev, (new_capacity + 2) * sizeof(size_t));

    if (list->data == NULL) return null_data;
    if (list->next == NULL) return null_next;
    if (list->prev == NULL) return null_prev;

    if (new_capacity > list->capacity) {
        list->next[list->last_free] = list->capacity + 2;
        list->last_free = new_capacity + 1;

        for (size_t i = list->capacity + 2; i < new_capacity + 1; i++) {
            list->data[i] = (TypeElem) Poison;
            list->next[i] = i + 1;
            list->prev[i] = Poison;
        }

        list->data[new_capacity + 1] = (TypeElem) Poison;
        list->next[new_capacity + 1] = list->last_free;
        list->prev[new_capacity + 1] = Poison;
    }
    
    list->capacity = new_capacity;

    if (list->size > list->capacity)
        list->size = list->capacity;

    return ok;
}

int list_dump_html(struct List *list, FILE *dump_file_html, const int line, const char *func, const char *file) {
    int check = list_ver(list);
    if (check) return check;

    static size_t num = 0;

    fprintf(dump_file_html, "<pre>\n");

    list_dump_file(list, dump_file_html, line, func, file);

    char graph_file_name_dot[13] = "graph000.dot";
    char graph_file_name_png[13] = "graph000.png";
                                  //01234567890123456789
    graph_file_name_dot[5] = num / 100 + '0';
    graph_file_name_png[5] = num / 100 + '0';
    graph_file_name_dot[6] = (num % 100) / 10 + '0';
    graph_file_name_png[6] = (num % 100) / 10 + '0';
    graph_file_name_dot[7] = num % 10 + '0';
    graph_file_name_png[7] = num % 10 + '0';

    list_dump_scheme(list, graph_file_name_dot);

    char to_system[40] = "dot -T png graph000.dot -o graph000.png";
                        //01234567890123456789012345678901234567890
    to_system[16] = num / 100 + '0';
    to_system[32] = num / 100 + '0';
    to_system[17] = (num % 100) / 10 + '0';
    to_system[33] = (num % 100) / 10 + '0';
    to_system[18] = num % 10 + '0';
    to_system[34] = num % 10 + '0';

    printf("%s\n", to_system);
    system(to_system);

    fprintf(dump_file_html, "<img src = \"%s\" />\n", graph_file_name_png);

    num++;

    return ok;
}

int list_dump_file(const struct List *list, FILE *dump_file, const int line, const char *func, const char *file) {
    int check = list_ver(list);
    if (check) return check;

    fprintf(dump_file, "In file %s in func %s at line %d\n", file, func, line);

    fprintf(dump_file, "is_ctor = %d\n", list->is_ctor);
    fprintf(dump_file, "is_dtor = %d\n", list->is_dtor);
    fprintf(dump_file, "capacity = %lu\n", list->capacity);
    fprintf(dump_file, "size = %lu\n", list->size);
    fprintf(dump_file, "free = %lu\n", list->free);
    fprintf(dump_file, "last_free = %lu\n", list->last_free);

    fprintf(dump_file, "list->data[%p]:\n", list->data);

    for (size_t i = 0; i < list->capacity + 2; i++)
        fprintf(dump_file, "%6lu ", i);
    
    fprintf(dump_file, "\n");

    for (size_t i = 0; i < list->capacity + 2; i++) {
        if ((list->data[i]) != Poison) {
            fprintf(dump_file, "%6d ", (int) list->data[i]);

        } else {
            fprintf(dump_file, "Poison ");
        }
    }

    fprintf(dump_file, "\n");

    fprintf(dump_file, "list->next[%p]:\n", list->next);

    for (size_t i = 0; i < list->capacity + 2; i++)
        fprintf(dump_file, "%6lu ", i);
    
    fprintf(dump_file, "\n");

    for (size_t i = 0; i < list->capacity + 2; i++) {
        if ((list->next[i]) != Poison) {
            fprintf(dump_file, "%6lu ", list->next[i]);

        } else {
            fprintf(dump_file, "Poison ");
        }
    }

    fprintf(dump_file, "\n");

    fprintf(dump_file, "list->prev[%p]:\n", list->prev);

    for (size_t i = 0; i < list->capacity + 2; i++)
        fprintf(dump_file, "%6lu ", i);
    
    fprintf(dump_file, "\n");

    for (size_t i = 0; i < list->capacity + 2; i++) {
        if ((list->prev[i]) != Poison) {
            fprintf(dump_file, "%6lu ", list->prev[i]);

        } else {
            fprintf(dump_file, "Poison ");
        }
    }

    fprintf(dump_file, "\n");

    return ok;
}

int list_dump_scheme(const struct List *list, const char *graph_file_name) {
    int check = list_ver(list);
    if (check) return check;

    FILE* graph_file = fopen(graph_file_name, "w");

    fprintf(graph_file, "digraph {\n");
    fprintf(graph_file, "\trankdir = TB;\n");
    fprintf(graph_file, "\tnode [shape = record];\n\n");

    fprintf(graph_file, "\tstruct0 [shape = record, label = \" {data\\[0\\] (Head) | %d |{<fp0> prev %lu | <fn0> next %lu}} \"]; \n", (int) list->data[0], list->prev[0], list->next[0]);
    fprintf(graph_file, "\tstruct1 [shape = record, label = \" {data\\[1\\] (Tail) | %d |{<fp1> prev %lu | <fn1> next %lu}} \"]; \n", (int) list->data[1], list->prev[1], list->next[1]);

    for (size_t i = 2; i < list->capacity + 2; i++) {
        if (list->data[i] != Poison) {
            if (list->data[i] != Poison) {
                fprintf(graph_file, "\tstruct%lu [shape = record, label = \" {data\\[%lu\\] | %d |{<fp%lu> prev %lu | <fn%lu> next %lu}} \"]; \n", i, i, (int) list->data[i], i, list->prev[i], i, list->next[i]);

            } else {
                fprintf(graph_file, "\tstruct%lu [shape = record, label = \" {data\\[%lu\\] | %d |{<fp%lu> prev Poison | <fn%lu> next %lu}} \"]; \n", i, i, (int) list->data[i], i, i, list->next[i]);
            }

        } else {
            if (list->data[i] != Poison) {
                fprintf(graph_file, "\tstruct%lu [shape = record, label = \" {data\\[%lu\\] | Poison |{<fp%lu> prev %lu | <fn%lu> next %lu}} \"]; \n", i, i, i, list->prev[i], i, list->next[i]);

            } else {
                fprintf(graph_file, "\tstruct%lu [shape = record, label = \" {data\\[%lu\\] | Poison |{<fp%lu> prev Poison | <fn%lu> next %lu}} \"]; \n", i, i, i, i, list->next[i]);
            }
        }
    }

    fprintf(graph_file, "\n");

    for (size_t i = 0; i < list->capacity + 2; i++)
        fprintf(graph_file, "\tstruct%lu:<fn%lu> -> struct%lu:<fp%lu> [color = \"red\"];\n", i, i, list->next[i], list->next[i]);

    fprintf(graph_file, "\n\tfree -> struct%lu:<fp%lu>\n", list->free, list->free);
    fprintf(graph_file, "\n\tlast_free -> struct%lu:<fp%lu>\n", list->last_free, list->last_free);

    fprintf(graph_file, "}\n");

    fclose(graph_file);
    
    return ok;
}

#ifdef DEBUGONL
void decrypt_errors(const int error, const char *name_called_func, const int line, const char *func, const char *file, FILE *err_file) {
    fprintf(err_file, "In file %s in func %s at line %d function %s returned ", file, func, line, name_called_func);

    switch(error) {

#define ERROR(a)                        \
    case a:                             \
        fprintf(err_file, #a "\n");     \
        break;                          

        ERROR(null_enter);
        ERROR(ok);
        ERROR(null_malloc);
        ERROR(null_data);
        ERROR(null_next);
        ERROR(null_prev);
        ERROR(just_ctor);
        ERROR(just_dtor);
        ERROR(dont_ctor);
        ERROR(size_capt);
        ERROR(impos_val_next);
        ERROR(impos_val_prev);
        ERROR(impos_val_free);
        ERROR(impos_val_last_free);
        ERROR(impos_enter_addres); 
        ERROR(just_free_elem);

#undef ERROR

        default:
            fprintf(err_file, "undefined_error\n");
    }
}

#endif
