#include "List.h"

int main() {
    printf("Test started...\n");

    struct List list = {};

    FILE *err_file = fopen("err.txt", "w");
    FILE* dump_htm = fopen("dump.htm", "w");

    DECRYPT_ERRORS(list_ctor(&list, 2), err_file);
    DECRYPT_ERRORS(list_ver(&list), err_file);
    DECRYPT_ERRORS(list_push_front(&list, 13), err_file);
    DECRYPT_ERRORS(list_push_front(&list, 14), err_file);
    DECRYPT_ERRORS(list_push_back(&list, 15), err_file);
    DECRYPT_ERRORS(list_dump_html(&list, dump_htm, __LINE__, __func__, __FILE__), err_file);
    DECRYPT_ERRORS(list_push_back(&list, 16), err_file);
    DECRYPT_ERRORS(list_push_back(&list, 17), err_file);
    DECRYPT_ERRORS(list_push_back(&list, 18), err_file);
    DECRYPT_ERRORS(list_push_back(&list, 19), err_file);
    DECRYPT_ERRORS(list_dump_html(&list, dump_htm, __LINE__, __func__, __FILE__), err_file);
    DECRYPT_ERRORS(list_push_front(&list, 20), err_file);
    DECRYPT_ERRORS(list_push_front(&list, 21), err_file);
    DECRYPT_ERRORS(list_push_front(&list, 22), err_file);

    //DECRYPT_ERRORS(list_dump(&list, __LINE__, __func__, __FILE__), err_file);

    DECRYPT_ERRORS(list_dump_html(&list, dump_htm, __LINE__, __func__, __FILE__), err_file);

    //list_dump_scheme(&list, "graph.dot");
    //system("dot -Tpng graph.dot -o p1.png");
    //printf("%lu\n", (size_t) -1);
    DECRYPT_ERRORS(list_dtor(&list), err_file);

    fclose(dump_htm);
    fclose(err_file);

    printf("Test comlited\n");

    return 0;
}