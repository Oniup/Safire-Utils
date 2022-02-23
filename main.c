// testing ... 

#include <stdio.h>
#include <stdlib.h>
#define SAFIRE_UTILS_IMPLEMENTATION
#include <safire-utils.h>
  
int main(int argc, char* argv[]) {
    
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
    return 0;
}
