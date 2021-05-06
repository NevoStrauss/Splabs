#include <stdio.h>

int main(int argc, char** argv){
    int iarray[] = {1,2,3};
    char carray[] = {'a','b','c'};
    int* iarrayPtr = iarray;
    char* carrayPtr = carray;
    int* p;

    printf("\n%s","|==================================================|");
    printf("\n  elems:     iarray   carray\n");
    for (int i = 0; i < 3; i++){
        printf("\n  Element %d:\t%i\t%c",i,iarrayPtr[i],carrayPtr[i]);
    }
    printf("\n%s","|==================================================|\n");
    printf("\n%s\t%p\n","The value of the pointer p is:",&p);
    return 0;
}
