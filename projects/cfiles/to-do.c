#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: to-do <task>\n");
        return 1;
    }
    char *test;
    test = (char *)malloc(100 * sizeof(char));
    scanf("%s", test);
    if (sizeof(test)<100){
        test = (char *)realloc(test,100*sizeof(char));
    }
    printf("%zu", sizeof(test));
    free(test);
    return 0;
}