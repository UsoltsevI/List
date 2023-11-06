#include "List.h"

int main() {
    printf("Test started...\n");

    struct List list = {};

    FILE *err_file = fopen("err.txt", "w");
    //FILE* dump_htm = fopen("/home/uso/C_projects/Ded/List/dump/dump.htm", "w");

    size_t test_addres = 0;

    DECRYPT_ERRORS(list_ctor(&list, 2), err_file);
    DECRYPT_ERRORS(list_ver(&list), err_file);
    DECRYPT_ERRORS(list_push_front(&list, 13, &test_addres), err_file);
    /*DECRYPT_ERRORS(list_push_front(&list, 14, &test_addres), err_file);
    DECRYPT_ERRORS(list_push_front(&list, 15, &test_addres), err_file);
    //DECRYPT_ERRORS(list_dump_html(&list, dump_htm, __LINE__, __func__, __FILE__), err_file);
    DECRYPT_ERRORS(list_push_back(&list, 16, &test_addres), err_file);
    DECRYPT_ERRORS(list_push_back(&list, 17, &test_addres), err_file);
    DECRYPT_ERRORS(list_push_back(&list, 18, &test_addres), err_file);
    DECRYPT_ERRORS(list_push_back(&list, 19, &test_addres), err_file);*/
    //DECRYPT_ERRORS(list_dump_html(&list, dump_htm, __LINE__, __func__, __FILE__), err_file);
    DECRYPT_ERRORS(list_push_front(&list, 20, &test_addres), err_file);
    DECRYPT_ERRORS(list_push_front(&list, 21, &test_addres), err_file);
    DECRYPT_ERRORS(list_push_front(&list, 22, &test_addres), err_file);

    //DECRYPT_ERRORS(list_dump(&list, __LINE__, __func__, __FILE__), err_file);

    //DECRYPT_ERRORS(list_dump_html(&list, dump_htm, __LINE__, __func__, __FILE__), err_file);

    DECRYPT_ERRORS(list_resize(&list, list.size + 1), err_file);
    DECRYPT_ERRORS(list_resize(&list, 2), err_file);

    //DECRYPT_ERRORS(list_dump_html(&list, dump_htm, __LINE__, __func__, __FILE__), err_file);

    //list_dump_scheme(&list, "graph.dot");
    //system("dot -Tpng graph.dot -o p1.png");
    //printf("%lu\n", (size_t) -1);

    DECRYPT_ERRORS(list_dtor(&list), err_file);

    //fclose(dump_htm);
    fclose(err_file);

    printf("Test comlited\n");

    return 0;
}