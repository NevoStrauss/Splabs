#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct fun_desc {
  char *name;
  char (*fun)(char);
};
 
char censor(char c) {
  if(c == '!')
    return '.';
  else
    return c;
}


 /* Gets a char c and returns its encrypted form by adding 3 to its value.
          If c is not between 0x20 and 0x7E it is returned unchanged */

char encrypt(char c){
    if (0x20<=c && c<=0x7E)
        return c+3;
    else
        return c;
}


/* Gets a char c and returns its decrypted form by reducing 3 to its value.
            If c is not between 0x20 and 0x7E it is returned unchanged */

char decrypt(char c){
    if (0x20<=c && c<=0x7E)
        return c-3;
    else
        return c;
}

/* dprt prints the value of c in a decimal representation followed by a
           new line, and returns c unchanged. */
// char dprt(char c){
//     printf("%d\n", c);
//     return c;
// }



/* If c is a number between 0x20 and 0x7E, cprt prints the character of ASCII value c followed
                    by a new line. Otherwise, cprt prints the dot ('.') character. After printing, cprt returns
                    the value of c unchanged. */
char cprt(char c){
    if(0x20<=c && c<=0x7E)
        printf("%c\n", c);
    else
        printf("%s",".\n");
    return c;
}


/* Ignores c, reads and returns a character from stdin using fgetc. */
char my_get(char c){
    return fgetc(stdin);
}


/* Gets a char c,  and if the char is 'q' , ends the program with exit code 0. Otherwise returns c. */
// char quit(char c){
//     if(c == 'q')
//         exit(0);
//     return c;
// }


char* map(char *array, int array_length, char (*f) (char)){
  char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
  for(int i = 0; i < array_length; i++){
      mapped_array[i] = (*f)(array[i]); // insert th i'th item in array to map_array after using function f;
  }
  return mapped_array;
}
 
int main(int argc, char **argv){
    //1 - define a char array initilaze to empty string
    char* carray = (char*)(malloc(5*sizeof(char)));
    //2 - delacrtion of an function array
    struct fun_desc menu[] = {{"Censor", censor}, {"Encrypt", encrypt},{"Decrypt", decrypt},
                              {"Print string", cprt},{ "Get string", my_get},{NULL, NULL}};


    int input;
    while (1){
        //3 - print the options
        size_t counter =0;
        while (menu[counter].name!=NULL){
            counter++;
        }
        size_t n =counter;
        //size_t n = sizeof(menu)/sizeof(menu[0]);// calculte the number of elements in the array
        printf("Please choose a function:\n");
        for(int i =0; i < n; i++){
            printf("%d) %s\n",i,menu[i].name);
        }
 
        //4 - ask for prompt and verify it
        printf("Option: ");
        scanf("%d",&input);
        fgetc(stdin); // clean the stdin from '\n'
        if (input>=0 && input<n){
            printf("Within bounds\n");
            // 5+6 - activate the apropiate function and continue
            char *tmp = carray;
            carray = map(carray, 5, menu[input].fun);
            free(tmp);
        }
        else{
            printf("Not within bounds\n");
            exit(0);
        }

        printf("DONE.\n\n");
    }
    free(carray);
    return 0;
}

