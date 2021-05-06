#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int PrintHex(char buffer[], size_t length)
{
    for (size_t i = 0; i < (length - 1); i++)
    {
        printf("%02hhX ", buffer[i]);
    }
    printf("%02hhX\n", buffer[length - 1]);
    return 0;
}

int main(int argc, char const *argv[])
{
    FILE *fp = fopen("infected", "r");
    // FILE *fp2 = fopen("sa2", "r");
    int length;

    char buffer[10000];
    // fread(buffer2, 1, 850, fp2);

    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    fread(buffer, 1, length, fp);
    fseek(fp, 0, SEEK_SET);

    // fseek(fp2, 0, SEEK_SET);
    // for (size_t i = 0; i < 840; i++)
    // {
    //     if (buffer[i] != buffer2[i])
    //     {
    //         printf("not equal:\nbuffer[%d]:%02hhX\nbuffer2[%d]%02hhX\n", i, buffer[i], i, buffer2[i]);
    //     }
    //     else
    //     {
    //         printf("%d-",i);
    //     }
    // }
    // printf("\n");

    PrintHex(buffer, length);
    // PrintHex(buffer2, 850);
    printf("this is length: %d\n", length);
    fclose(fp);
    return 0;
}
