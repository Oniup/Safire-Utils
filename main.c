#include <stdio.h>
#include <stdlib.h>
#include <safire-utils.h>

int main(int argc, char* argv[]) {
    char* str = sfr_str("this is a test");

    printf("%s", str);

    sfr_str_free(&str);

    return 0;
}