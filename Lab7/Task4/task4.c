#include <stdio.h>

int digit_cnt(char *sequence){
    int counter = 0;
    int i = 0;
    while(sequence[i] != '\0'){
        if(sequence[i] >= '0' && sequence[i]<= '9')
            counter = counter+1;
        i = i+1;
    }
    return counter;
}

int main(int argc, char **argv){
    int count = digit_cnt(argv[1]);
    printf("%d\n",count);
}