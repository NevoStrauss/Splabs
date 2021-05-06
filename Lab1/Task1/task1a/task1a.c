#include <stdio.h>

int main(int argc, char **argv){
    unsigned int read;
    while ((read = fgetc(stdin)) != EOF){
            if (read>='A' && read<='Z')
                read = read+32; // also possible: read += 'a'-'A'
            printf("%c",read);
        }
    return 0;
}