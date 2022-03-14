// testing ... 

#include <stdio.h>
#include <stdlib.h>
#define SAFIRE_UTILS_IMPLEMENTATION
#include <safire-utils.h>
  
void string_test();
void list_test();
void config_test();
void sorting_test();

int main(int argc, char* argv[]) {
   printf("\n\n======== string test ========\n");
   string_test();
   printf("\n\n========= list test =========\n");
   list_test();
   printf("\n\n======== config test ========\n");
   config_test();
   printf("\n\n======= sorting test ========\n");
   sorting_test();
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
   uint32_t list_size = 5;
   uint32_t* list = SFR_LIST_create(uint32_t, list_size);
   for (uint32_t i = 0; i < list_size; i++) {
      list[i] = i;
   }
   SFR_LIST_push(uint32_t, list, list_size, 69);

   printf("full dynamic array (%u):\n", list_size);
   for (uint32_t i = 0; i < list_size; i++) {
      printf("%u ", list[i]);
   }
   printf("\n");

   SFR_LIST_erase(uint32_t, list, list_size, 1);
   SFR_LIST_pop(uint32_t, list, list_size);

   printf("removed the 2nd element and popped the last (%u):\n", list_size);
   for (uint32_t i = 0; i < list_size; i++) {
      printf("%u ", list[i]);
   }
   printf("\n");

   uint32_t list2_size = 0;
   uint32_t* list2 = NULL;
   SFR_LIST_copy(uint32_t, list2, list2_size, list, list_size);

   printf("copied the smaller array to another array:\n");
   for (uint32_t i = 0; i < list2_size; i++) {
      printf("%u ", list2[i]);
   }
   printf("\n");


   SFR_LIST_free(list, list_size);
   SFR_LIST_free(list2, list2_size);
}

void config_test() {
   SFR_config_t config = {};
   sfr_config_read(&config, "testing/test-config-data.config");
   sfr_config_print(&config);
   sfr_config_free(&config);
}

void sorting_test() {
   int size = 10;
   int array[10] = { 32, 13, 4, 31, 3, 123, 34, 34, 76, 54 };
   printf("un-sorted array:\n");
   for (uint32_t i = 0; i < size; i++) {
      printf("%u ", array[i]);
   }
   printf("\nsorted array:\n");
   sfr_quick_sort(array, size);
   for (uint32_t i = 0; i < size; i++) {
      printf("%u ", array[i]);
   }
   printf("\n");
}
