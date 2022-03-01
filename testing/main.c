// testing ... 

#include <stdio.h>
#include <stdlib.h>
#define SAFIRE_UTILS_IMPLEMENTATION
#include <safire-utils.h>
  
void string_test();
void list_test();
void config_test();

int main(int argc, char* argv[]) {
    printf("\n\n");
    string_test();
    printf("\n\n");
    list_test();
    config_test();
    return 0;
}

void string_test() {
    char* string = sfr_str("this is a test");
    printf("string initialisation: %s\n", string);
    sfr_str_set(string, "copying another string to the original");
    printf("second string test: %s\n", string);
    sfr_str_free(&string);
}

void list_test() {
    uint32_t list_size = 0;
    uint32_t* list = NULL;
    SFR_LIST_push(uint32_t, list, list_size, 43);
    SFR_LIST_push(uint32_t, list, list_size, 44);
    SFR_LIST_push(uint32_t, list, list_size, 45);
    SFR_LIST_push(uint32_t, list, list_size, 46);

    printf("first:\n");
    for (uint32_t i = 0; i < list_size; i++) {
        printf("%u\n", list[i]);
    }

    SFR_LIST_erase(uint32_t, list, list_size, 1);
    SFR_LIST_pop(uint32_t, list, list_size);

    printf("second:\n");
    for (uint32_t i = 0; i < list_size; i++) {
        printf("%u\n", list[i]);
    }

    uint32_t list2_size = 0;
    uint32_t* list2 = NULL;
    SFR_LIST_copy(uint32_t, list2, list2_size, list, list_size);

    printf("copied:\n");
    for (uint32_t i = 0; i < list2_size; i++) {
        printf("%u\n", list2[i]);
    }

    SFR_LIST_free(list, list_size);
    SFR_LIST_free(list2, list2_size);
}

void config_test() {
    SFR_config_t config = {};
    sfr_config_read(&config, "testing/test-config-data.config");
    sfr_config_print(&config);
    sfr_config_free(&config);
}
