#include <stdio.h>
#include <stdlib.h>

int PrintHex(char buffer[], size_t length){
    for (size_t i = 0; i < (length - 1); i++){
        printf("%02hhX ", buffer[i]);
    }
    printf("%02hhX\n", buffer[length - 1]);
    return 0;
}

int main(int argc, char **argv){
    FILE *fp;
    fp = fopen(argv[1], "r");

    if (fp == NULL){
        printf("\nError:! cannot open file");
        exit(1);
    }
    fseek(fp, 0, SEEK_END);
    size_t length = (long)ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *buffer;
    buffer = (char *)malloc(length);
    fread(buffer, 1, length, fp);
    PrintHex(buffer, length);
    free(buffer);
    fclose(fp);
    return 0;
}