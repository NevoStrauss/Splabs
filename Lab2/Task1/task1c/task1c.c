#include <stdio.h>
#include <string.h>

int main(int argc, char **argv){
    int iarray[3];    //4 bytes between each cell
    float farray[3];  //4 bytes between each cell  
    double darray[3]; //8 bytes between each cell
    char carray[3];   //1 byte between each cell
    int* iptrarray[3];
    char* cptrarray[3];
    int *p;

    //first part:
    printf("\n%s","|================================================================================================================|");
    printf("\n  Arr's elems:    iarray          farray          darray          carray          iptr            cptr\n");
    for (int i = 0; i < 3; i++){
        printf("\n  Element %d:\t%p\t%p\t%p\t%p\t%p\t%p",i,&iarray[i],&farray[i],&darray[i],&carray[i],&iptrarray[i],&cptrarray[i]);
    }
    printf("\n%s","|================================================================================================================|\n");
    
    //second part:
    printf("\nhex val of iarray      : \t%p", &iarray);
    printf("\nhex val of iarray+1    : \t%p", (&iarray+1));
    printf("\nhex val of farray      : \t%p", &farray);
    printf("\nhex val of farray+1    : \t%p", (&farray+1));
    printf("\nhex val of darray      : \t%p", &darray);
    printf("\nhex val of darray+1    : \t%p", (&darray+1));
    printf("\nhex val of carray      : \t%p", &carray);
    printf("\nhex val of carray+1    : \t%p", (&carray+1));
    printf("\nhex val of iptrarray   : \t%p", iptrarray);
    printf("\nhex val of iptrarray+1 : \t%p", (iptrarray+1));
    printf("\nhex val of cptrarray   : \t%p", cptrarray);
    printf("\nhex val of cptrarray+1 : \t%p\n", (cptrarray+1));
    printf("\nhex val of pointer p   :\t%p\n",&p);
    //observe that it below the arrays because it uninitilized
    //Conclusions:
    //The + operator increases the pointer value by the size of the wholw array.
    return 0;
}