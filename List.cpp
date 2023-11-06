#include "List.h"

#ifdef DEBUGONL

FILE *list_dump_file_html = {};
int NUM_ERRORS = 0;
#define DUMP_OPEN list_dump_file_html = fopen("dump/dump_file_html.htm", "w");
#define DUMP_CLOSE fprintf(list_dump_file_html, "<font color = \"#ff0000\" size = \"7\"> NUM_ERRORS: %d </font>\n", NUM_ERRORS); fclose(list_dump_file_html);
#define DUMP_ERROR(err) fprintf(list_dump_file_html, "<font color = \"#ff0000\" size = \"7\">ERROR in func %s, in file %s, on line %d:</font>" \
                                                        "<font color = \"#cc0088\" size = \"7\"> %s </font>\n", __func__, __FILE__, __LINE__, #err); NUM_ERRORS++;
#define DUMP list_dump_html(list, list_dump_file_html, __LINE__, __func__, __FILE__);  
#else 

#define DUMP_OPEN ;
#define DUMP ;
#define DUMP_ERROR ;
#define DUMP_CLOSE ;

#endif 

static int list_resize_above_size(struct List *list, const size_t new_capacity);
static int list_stack_free(struct List *list, size_t last_acceptable_addres);
static int list_stack(struct List *list, size_t last_acceptable_addres);

int list_ctor(struct List *list, size_t beg_capacity) {
    DUMP_OPEN;

    if (list == NULL) {
        DUMP_ERROR(null_enter);
        return null_enter;
    }
    
    if (list->is_dtor == 1) {
        DUMP_ERROR(just_dtor);
        return just_dtor;
    }

    if (list->is_ctor == 1) {
        DUMP_ERROR(just_ctor);
        return just_ctor;
    }

    list->data = (TypeElem *) calloc(beg_capacity + 2, sizeof(TypeElem));
    list->next = (size_t *) calloc(beg_capacity + 2, sizeof(size_t));
    list->prev = (size_t *) calloc(beg_capacity + 2, sizeof(size_t));

    if (list->data == NULL) {
        DUMP_ERROR(null_data);
        return null_data;
    }
    
    if (list->next == NULL) {
        DUMP_ERROR(null_next);
        return null_next;
    }
    
    if (list->prev == NULL) {
        DUMP_ERROR(null_next);
        return null_prev;
    }

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

    DUMP;

    return ok;
}

int list_dtor(struct List *list) {
    int check = list_ver(list);
    if (check) { 
        DUMP_CLOSE;
        return check;
    }

    DUMP;
    DUMP_CLOSE;

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
    if (list == NULL) {
        DUMP_ERROR(null_enter);
        return null_enter;
    }
    
    if (list->is_dtor == 1) {
        DUMP_ERROR(just_dtor);
        return just_dtor;
    }

    if (list->is_ctor == 0) {
        DUMP_ERROR(dont_ctor);
        return dont_ctor;
    }
    
    if (list->data == NULL) {
        DUMP_ERROR(null_data);
        return null_data;
    }
    
    if (list->next == NULL) {
        DUMP_ERROR(null_next);
        return null_next;
    }
    
    if (list->prev == NULL) {
        DUMP_ERROR(null_prev);
        return null_prev;
    }
    
    if (list->size > list->capacity) {
        DUMP_ERROR(size_capt);
        return size_capt;
    }

    for (size_t i = 0; i < list->capacity + 2; i++) {
        if (list->next[i] >= list->capacity + 2) {
            DUMP_ERROR(impos_val_next);
            return impos_val_next;
        }
        
        if ((list->prev[i] >= list->capacity + 2) && (list->prev[i] != Poison)) {
            DUMP_ERROR(impos_val_prev);
            return impos_val_prev;
        }
    }
    
    if ((list->free >= list->capacity + 2) || (list->free < 2)) {
        DUMP_ERROR(impos_val_free);
        return impos_val_free;
    }
    
    if ((list->last_free >= list->capacity + 2) || (list->free < 2)) {
        DUMP_ERROR(impos_val_last_free);
        return impos_val_last_free;
    }

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

int list_push_front(struct List *list, const TypeElem new_elem, size_t *ret_addres) {
    return list_push_after(list, list->head, new_elem, ret_addres);
}

int list_push_back(struct List *list, const TypeElem new_elem, size_t *ret_addres) {
    return list_push_after(list, list->prev[list->tail], new_elem, ret_addres);
}

int list_push_before(struct List *list, const size_t addres_ref_elem, const TypeElem new_elem, size_t *ret_addres) { 
    return list_push_after(list, list->prev[addres_ref_elem], new_elem, ret_addres);
}

int list_push_after(struct List *list, const size_t addres_ref_elem, const TypeElem new_elem, size_t *ret_addres) {
    int check = list_ver(list);
    if (check) return check;

    conditional_realloc_increase(list);

    if (addres_ref_elem >= list->capacity + 2)
        return impos_enter_addres;

    size_t addres_new_elem = list->free;
    list->free = list->next[list->free]; 
    list->data[addres_new_elem] = new_elem;
    list->next[addres_new_elem] = list->next[addres_ref_elem];
    list->prev[addres_new_elem] = addres_ref_elem;
    list->prev[list->next[addres_ref_elem]] = addres_new_elem;
    list->next[addres_ref_elem] = addres_new_elem;
    list->size++;

    if (list->size == 1)
        list->prev[list->tail] = addres_new_elem;
    
    *ret_addres = addres_new_elem;

    DUMP;

    return ok;
}

int list_remove(struct List *list, const size_t addres_rem_elem) {
    int check = list_ver(list);
    if (check) return check;

    if ((addres_rem_elem >= list->capacity) || (list->prev[addres_rem_elem] == Poison)) {
        DUMP_ERROR(impos_enter_addres);
        return impos_enter_addres;
    }

    if (list->prev[addres_rem_elem] == Poison) {
        DUMP_ERROR(just_free_elem);
        return just_free_elem;
    }

    list->prev[list->next[addres_rem_elem]] = list->prev[addres_rem_elem];
    list->next[list->prev[addres_rem_elem]] = list->next[addres_rem_elem];
    list->next[list->last_free] = addres_rem_elem;
    list->last_free = addres_rem_elem;
    list->next[addres_rem_elem] = list->last_free;
    list->prev[addres_rem_elem] = Poison;
    list->data[addres_rem_elem] = (TypeElem) Poison;

    list->size--;

    DUMP;

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
    size_t count = 0;

    while (count < num) {    
        i = list->next[i];
        count++;
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

    if (list->data == NULL) {
        DUMP_ERROR(null_data);
        return null_data;
    }

    if (list->next == NULL) {
        DUMP_ERROR(null_next)
        return null_next;
    }
    if (list->prev == NULL) {
        DUMP_ERROR(null_prev);
        return null_prev;
    }

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

int list_resize(struct List *list, const size_t new_capacity) {
    int check = list_ver(list);
    if (check) return check;

    if (new_capacity > list->size)
        return list_resize_above_size(list, new_capacity);

    if (check = list_align_ids_by_addresses(list))
        return check;

    list->next[new_capacity + 1] = list->tail; 
    list->prev[list->tail] = new_capacity + 1;
    list->free = new_capacity + 2;
    list->last_free = list->free;
    list->next[list->last_free] = list->last_free;
    list->size = new_capacity;

    if (check = list_realloc(list, new_capacity + 1))
        return check;
    
    DUMP;

    return ok;
}

int list_resize_above_size(struct List *list, const size_t new_capacity) {
    int check = list_ver(list);
    if (check) return check;

    if (new_capacity >= list->capacity)
        return list_realloc(list, new_capacity);
    
    if (new_capacity < list->size + 1) {
        DUMP_ERROR(impos_new_cap);
        return impos_new_cap;
    }

    size_t cur_pos = list->next[list->head];

    if (check = list_stack(list, new_capacity + 2))
        return check;
    
    if (check = list_realloc(list, new_capacity))
        return check;

    list->next[new_capacity + 1] = list->tail; 
    list->last_free = new_capacity + 1; 
    //list->last_free = list->free;
    list->next[list->last_free] = list->last_free;

    DUMP;

    return ok;
}

int list_align_ids_by_addresses(struct List *list) {
    int check = list_ver(list);
    if (check) return check;

    for (size_t cur_id = 0, cur_addres = list->next[list->head]; (cur_id < list->size) && (cur_addres != list->tail); cur_id++) {
        if (cur_addres != cur_id + 2)
            if (check = list_swap_places_by_addresses(list, cur_addres, cur_id + 2))
                return check;

        cur_addres = list->next[cur_id + 2];
    }

    DUMP;

    return ok;
}

int list_stack(struct List *list, size_t last_acceptable_addres) {
    int check = list_ver(list);
    if (check) return check;
    
    if (check = list_stack_free(list, last_acceptable_addres))
        return check;

    size_t cur_pos = list->next[list->head];
    size_t cur_free = list->free;

    for (size_t i = 0; i < list->size; i++) {
        if (cur_pos > list->size + 1) {
            while (cur_free > list->size + 1)
                cur_free = list->next[cur_free];

            if (check = list_swap_places_by_addresses(list, cur_pos, cur_free))
                return check;
        }

        //list->free = list->next[list->free]; in loop?

        cur_pos = list->next[cur_pos];
    }
    
    DUMP;

    return ok;
}

int list_stack_free(struct List *list, size_t last_acceptable_addres) {
    int check = list_ver(list);
    if (check) return check;

    if (last_acceptable_addres < list->size + 2) {
        DUMP_ERROR(impos_enter_addres);
        return impos_enter_addres;
    }

    while (list->free > last_acceptable_addres)
        list->free = list->next[list->free];

    size_t num_free = last_acceptable_addres - list->size - 1;
    size_t cur_num_free = 1;
    size_t cur_addres_free = list->free;
    size_t score = 0;

    while (cur_num_free < num_free) {
        if (list->next[cur_addres_free] > last_acceptable_addres) {
            list->next[cur_addres_free] = list->next[list->next[cur_addres_free]];
            score++;

            if (score > list->capacity - list->size) {
                DUMP_ERROR(path_to_eternity);
                return path_to_eternity;
            }

        } else {
            cur_num_free++;
            cur_addres_free = list->next[cur_addres_free];
        }
    }

    //list->next[list->free] = cur_addres_free;
    //list->last_free = list->last_free;
    list->last_free = cur_addres_free;
    list->next[list->last_free] = list->last_free;

    DUMP;
    
    return ok;
}

int list_swap_places_by_addresses(struct List *list, const size_t pos1, const size_t pos2) {
    int check = list_ver(list);
    if (check) return check;

    if ((pos1 == list->head) || (pos2 == list->head)) {
        DUMP_ERROR(impos_enter_addres);
        return impos_enter_addres;
    }

    if ((pos1 == list->tail) || (pos2 == list->tail)) {
        DUMP_ERROR(impos_enter_addres);
        return impos_enter_addres;
    }

    if ((pos1 > list->capacity + 1) || (pos2 > list->capacity + 1)) {
        DUMP_ERROR(impos_enter_addres);
        return impos_enter_addres;
    }

    if (pos1 == pos2) 
        return ok;
    
    if ((list->prev[pos1] == Poison) && (list->prev[pos2] == Poison))
        return ok;
    
    TypeElem data_h = list->data[pos1];
    size_t next_h = list->next[pos1];
    size_t prev_h = list->prev[pos1];

    list->data[pos1] = list->data[pos2];
    list->data[pos2] = data_h; 

    if (list->free == pos1) {
        list->free = pos2;
    
    } else if (list->free == pos2) {
        list->free = pos1;
    }

#ifdef DEBUGONL
    fprintf(list_dump_file_html, "<font color = \"blue\" size = \"6\">\n SWAP: pos1 = %lu, pos2 = %lu \n</font>", pos1, pos2);
#endif

    if (list->next[pos1] == pos2) {
        if (list->prev[pos1] != Poison)
            list->next[list->prev[pos1]] = pos2;
        
        list->prev[list->next[pos2]] = pos1;

        list->prev[pos1] = pos2;

        list->prev[pos2] = prev_h;

        list->next[pos1] = list->next[pos2];
        list->next[pos2] = pos1;

    } else if (list->next[pos2] == pos1) {
        if (list->prev[pos2] != Poison)
            list->next[list->prev[pos2]] = pos1;
        
        list->prev[list->next[pos1]] = pos2;
        list->prev[pos2] = pos1;

        list->prev[pos1] = list->prev[pos2];
        list->next[pos2] = next_h;
        list->next[pos1] = pos2;

    } else {
        if (list->prev[pos1] != Poison)
            list->next[list->prev[pos1]] = pos2;

        if (list->prev[pos2] != Poison)
            list->next[list->prev[pos2]] = pos1;
        
        if (list->prev[list->next[pos1]] != Poison)
            list->prev[list->next[pos1]] = pos2;

        if (list->prev[list->next[pos2]] != Poison)
            list->prev[list->next[pos2]] = pos1;

        list->prev[pos1] = list->prev[pos2]; 
        list->prev[pos2] = prev_h;

        list->next[pos1] = list->next[pos2];
        list->next[pos2] = next_h;

        if (list->last_free == pos2) {
            size_t cur_num = list->free;
            while (list->next[cur_num] != list->last_free)
                cur_num = list->next[cur_num];
            
            list->next[cur_num] = pos1;
            list->last_free = pos1;
            list->next[list->last_free] = pos1;

        } else if (list->last_free == pos1) {
            size_t cur_num = list->free;
            while (list->next[cur_num] != list->last_free)
                cur_num = list->next[cur_num];
            
            list->next[cur_num] = pos2;
            list->last_free = pos2;
            list->next[list->last_free] = pos2;

        } else if (list->prev[pos1] == Poison) {
            list->next[list->last_free] = pos1;
            list->last_free = pos1;
            list->next[list->last_free] = pos1;

        } else if (list->prev[pos2] == Poison) {
            list->next[list->last_free] = pos2;
            list->last_free = pos2;
            list->next[list->last_free] = pos2;
        }

    }

    DUMP;

    return ok;
}

int list_dump_html(struct List *list, FILE *dump_file_html, const int line, const char *func, const char *file) {
    int check = list_ver(list);
    if (check) return check;

    if (dump_file_html == NULL) {
        DUMP_ERROR(null_file);
        return null_file;
    }

    static size_t num = 0;

    fprintf(dump_file_html, "<pre>\n");
    fprintf(dump_file_html, "<font size = \"5\">\n");

    list_dump_file(list, dump_file_html, line, func, file);

    const int name_file_len = 20;
    char graph_file_name_dot[name_file_len] = "dump/graph000.dot";
    char graph_file_name_png[name_file_len] = "dump/graph000.png";
                                             //0123456789012345678901234567890
                                             //0         1         2      
    graph_file_name_dot[10] = num / 100 + '0';
    graph_file_name_png[10] = num / 100 + '0';
    graph_file_name_dot[11] = (num % 100) / 10 + '0';
    graph_file_name_png[11] = (num % 100) / 10 + '0';
    graph_file_name_dot[12] = num % 10 + '0';
    graph_file_name_png[12] = num % 10 + '0';

    LIST_DUMP_SCHEME(list, graph_file_name_dot);

    const int command_system_len = 50;
    char to_system[command_system_len] = "dot -T png dump/graph000.dot -o dump/graph000.png";
                         //0123456789012345678901234567890123456789012345678901234567890
                         //0         1         2         3         4         5  
    to_system[21] = num / 100 + '0';
    to_system[42] = num / 100 + '0';
    to_system[22] = (num % 100) / 10 + '0';
    to_system[43] = (num % 100) / 10 + '0';
    to_system[23] = num % 10 + '0';
    to_system[44] = num % 10 + '0';

    printf("%s\n", to_system);
    system(to_system);

    fprintf(dump_file_html, "</font>\n");

    fprintf(dump_file_html, "<img src = \"%s\" />\n", &graph_file_name_png[5]);

    num++;

    return ok;
}

int list_dump_file(const struct List *list, FILE *dump_file, const int line, const char *func, const char *file) {
    int check = list_ver(list);
    if (check) return check;

    if (dump_file == NULL) {
        DUMP_ERROR(null_file);
        return null_file;
    }

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

int list_dump_scheme(const struct List *list, const char *graph_file_name, const char *list_name) {
    int check = list_ver(list);
    if (check) return check;

    if (graph_file_name == NULL) return null_file_name;

    FILE* graph_file = fopen(graph_file_name, "w");

    fprintf(graph_file, "digraph {\n");
    fprintf(graph_file, "\trankdir = TB;\n");
    fprintf(graph_file, "\tnode [shape = record];\n\n");

    fprintf(graph_file, "\tList [shape = \"record\", style = \"filled\", fillcolor = \"#e5ff15\",  label = \"{ name: %s | size: %lu | capacity: %lu | <H> head: %lu | <T> tail: %li | <F> free: %lu | <LF> last_free: %lu} \"]\n\n", list_name, list->size, list->capacity, list->head, list->tail, list->free, list->last_free);

    fprintf(graph_file, "\tstruct0 [shape = record, style = \"filled\", fillcolor = \"#8888ff\", label = \" {data\\[0\\] (Head) | %d |{<fp0> prev %lu | <fn0> next %lu}} \"]; \n", (int) list->data[list->head], list->prev[list->head], list->next[list->head]);
    fprintf(graph_file, "\tstruct1 [shape = record, style = \"filled\", fillcolor = \"#ff5555\", label = \" {data\\[1\\] (Tail) | %d |{<fp1> prev %lu | <fn1> next %lu}} \"]; \n\n", (int) list->data[list->tail], list->prev[list->tail], list->next[list->tail]);

    fprintf(graph_file, "\tList:<H> -> struct0 [color = \"#8888ff\"];\n");
    fprintf(graph_file, "\tList:<T> -> struct1 [color = \"#ff5555\"];\n");
    fprintf(graph_file, "\tList:<F> -> free [color = \"#6666aa\"];\n");
    fprintf(graph_file, "\tList:<LF> -> last_free [color = \"#cc5555\"];\n\n\n");

    fprintf(graph_file, "\tstruct%lu:<fn%lu> -> struct%lu:<fn%lu> [color = \"blue\"];\n", list->head, list->head, list->next[list->head], list->next[list->head]);
    fprintf(graph_file, "\tstruct%lu:<fn%lu> -> struct%lu:<fn%lu> [color = \"blue\"];\n", list->tail, list->tail, list->next[list->tail], list->next[list->tail]);
    fprintf(graph_file, "\tstruct%lu:<fp%lu> -> struct%lu:<fp%lu> [color = \"red\"];\n", list->head, list->head, list->prev[list->head], list->prev[list->head]);
    fprintf(graph_file, "\tstruct%lu:<fp%lu> -> struct%lu:<fp%lu> [color = \"red\"];\n\n\n", list->tail, list->tail, list->prev[list->tail], list->prev[list->tail]);

    size_t cur_pos = list->next[list->head];

    for (size_t i = 0; i < list->size; i++) {
        if (list->data[cur_pos] != Poison) {
            fprintf(graph_file, "\tstruct%lu [shape = record, style = \"filled\", fillcolor = \"#8ef482\", label = \" {data\\[%lu\\] | id %lu | %d |{<fp%lu> prev %lu | <fn%lu> next %lu}} \"]; \n", cur_pos, cur_pos, i, (int) list->data[cur_pos], cur_pos, list->prev[cur_pos], cur_pos, list->next[cur_pos]);
            fprintf(graph_file, "\tstruct%lu:<fn%lu> -> struct%lu:<fn%lu> [color = \"blue\"];\n", cur_pos, cur_pos, list->next[cur_pos], list->next[cur_pos]);
            fprintf(graph_file, "\tstruct%lu:<fp%lu> -> struct%lu:<fp%lu> [color = \"red\"];\n\n", cur_pos, cur_pos, list->prev[cur_pos], list->prev[cur_pos]);

        } else {
            fprintf(graph_file, "\tstruct%lu [shape = record, style = \"filled\", fillcolor = \"#8ef482\", label = \" {data\\[%lu\\] | id %lu | Poison |{<fp%lu> prev Poison | <fn%lu> next %lu}} \"]; \n", cur_pos, cur_pos, i, cur_pos, cur_pos, list->next[cur_pos]);
            fprintf(graph_file, "\tstruct%lu:<fn%lu> -> struct%lu:<fn%lu> [color = \"blue\"];\n", cur_pos, cur_pos, list->next[cur_pos], list->next[cur_pos]);
            fprintf(graph_file, "\tstruct%lu:<fp%lu> -> struct%lu:<fp%lu> [color = \"red\"];\n\n", cur_pos, cur_pos, list->prev[cur_pos], list->prev[cur_pos]);
        }

        cur_pos = list->next[cur_pos];
    }

    fprintf(graph_file, "\n");
    
    cur_pos = list->free;

    for (size_t i = 0; i < list->capacity - list->size; i++) {
        if (list->data[cur_pos] != Poison) {
            fprintf(graph_file, "\tstruct%lu [shape = record, style = \"filled\", fillcolor = \"#88bb88\", label = \" {data\\[%lu\\] | %d |{<fp%lu> prev %lu | <fn%lu> next %lu}} \"]; \n", cur_pos, cur_pos, (int) list->data[cur_pos], cur_pos, list->prev[cur_pos], cur_pos, list->next[cur_pos]);
            fprintf(graph_file, "\tstruct%lu:<fn%lu> -> struct%lu:<fp%lu> [color = \"#115511\"];\n\n", cur_pos, cur_pos, list->next[cur_pos], list->next[cur_pos]);

        } else {
            fprintf(graph_file, "\tstruct%lu [shape = record, style = \"filled\", fillcolor = \"#88bb88\", label = \" {data\\[%lu\\] | Poison |{<fp%lu> prev Poison | <fn%lu> next %lu}} \"]; \n", cur_pos, cur_pos, cur_pos, cur_pos, list->next[cur_pos]);
            fprintf(graph_file, "\tstruct%lu:<fn%lu> -> struct%lu:<fp%lu> [color = \"#115511\"];\n\n", cur_pos, cur_pos, list->next[cur_pos], list->next[cur_pos]);
        }

        cur_pos = list->next[cur_pos];
    }

    fprintf(graph_file, "\n\tfree [shape = record, style = \"filled\", fillcolor = \"#6666aa\", label = \"free\"]; \n");
    fprintf(graph_file, "\tlast_free [shape = record, style = \"filled\", fillcolor = \"#cc5555\", label = \"last_free\"];\n");

    fprintf(graph_file, "\tfree -> struct%lu:<fp%lu> [color = \"#6666aa\"];\n", list->free, list->free);
    fprintf(graph_file, "\tlast_free -> struct%lu:<fp%lu> [color = \"#cc5555\"];\n", list->last_free, list->last_free);

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
        fprintf(err_file, "%s\n", #a);  \
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
        ERROR(null_file);
        ERROR(null_file_name);
        ERROR(impos_new_cap);
        ERROR(path_to_eternity);
        ERROR(fall_from_finding_suitable_free_space);

#undef ERROR

        default:
            fprintf(err_file, "undefined_error\n");
    }
}

#endif
