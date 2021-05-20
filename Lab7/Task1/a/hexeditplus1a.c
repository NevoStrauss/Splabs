#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define  NAME_LEN  128
#define  BUF_SZ    10000

typedef struct state{
  char debug_mode;
  char file_name[NAME_LEN];
  int unit_size;
  unsigned char mem_buf[BUF_SZ];
  size_t mem_count;
} state;

typedef struct fun_desc{
  char *name;
  void (*fun)(state*);
}fun_desc;

void Toggle_Debug_Mode(state* s){
    if(s->debug_mode == 'F'){
        s->debug_mode = 'T';
        printf("%s\n","Debug flag now on");
    }
    else if(s->debug_mode == 'T'){
        s->debug_mode = 'F';
        printf("%s\n","Debug flag now off");
    }
}

void Set_File_Name(state* s){
    printf("%s","Enter new file name: ");
    scanf("%s",s->file_name);
    if(s->debug_mode=='T')
        printf("%s%s\n","Debug: file name set to ",s->file_name);
}

void Set_Unit_Size(state* s){
    int newSize;
    scanf("%d",&newSize);
    fgetc(stdin); // clean the stdin from '\n'
    if(newSize==1 || newSize==2 || newSize==4){
        s->unit_size = newSize;
        if(s->debug_mode == 'T')
            printf("%s%d\n","Debug: set size to ",newSize);
    }
    else
        printf("%s\n","Invalid unit_size input, should be 1, 2 or 4");
}

void Load_Into_Memory(state* s){
    char* fileName = s->file_name;
    if(strcmp(fileName,"")==0){
        printf("%s","Error: file name is empty");
        return;
    }
    int fds = open(fileName, O_RDONLY);
    if(fds == -1){
        printf("\n%s %s\n","Error opening the file: ",fileName);
        return;
    }

    int location, length;
    char arguments[10000];

    printf("%s\n","Enter location(Hex) and length(Dec): ");
    scanf("%x %d", &location, &length);
    //fgets(arguments,10000,stdin);
    //scanf(arguments,"%x %d",&location, &length);

    if(s->debug_mode == 'T'){
        printf("\n%s%s\n","File name: ",s->file_name);
        printf("%s%x\n","Location: ",location);
        printf("%s%d\n","Length: ",length);
    }

    // load into the state buffer the memory of the file
    s->mem_count = length * (s->unit_size);         // update the counter field
    lseek(fds, location, SEEK_SET);                 // move to the memory location specified
    read(fds, s->mem_buf, (length * s->unit_size)); // read the memory to the buffer
    printf("Loaded %d units into memory\n", length);

    close(fds);
}

void Memory_Display(state* s){
    printf("%s","Memory_Display");
}

void Save_Into_File(state* s){
    printf("%s\n","Save_Into_File");

}

void Memory_Modify(state* s){
    printf("%s\n","Memory_Modify");
}

void Quit(state* s){
    if(s->debug_mode == 'T')
        printf("\n%s\n","quitting..");
    exit(0);
}

void debug_print_members(state* s){
    printf("%s%d\n","Unit size: ",s->unit_size);
    printf("%s%s\n","File name: ",s->file_name);
    printf("%s%d\n","Mem count: ",s->mem_count);
}

int main(int argc, char **argv){
    fun_desc menu[] = {{"Toggle Debug Mode", Toggle_Debug_Mode},
                       {"Set File Name", Set_File_Name},
                       {"Set Unit Size", Set_Unit_Size},
                       {"Load Into Memory", Load_Into_Memory},
                       {"Memory Display", Memory_Display},
                       {"Save Into File", Save_Into_File},
                       {"Memory Modify", Memory_Modify},
                       {"Quit", Quit},
                       {NULL, NULL}};
    
    state my_state;
    //calculte the number of elements in the menu array
    size_t size = sizeof(menu)/sizeof(menu[0]);
    char* carray = (char*)(malloc(size*sizeof(char)));
    my_state.debug_mode = 'F';
    my_state.unit_size = 1;
    my_state.mem_count = 0;
    int input;

    while (true){
        //if debug mode, print the members:
        if(my_state.debug_mode == 'T')
            debug_print_members(&my_state);

        printf("\nChoose action:\n");
        for(int i=0; i < size; i++){
            if(menu[i].name != NULL)
                printf("%d- %s\n",i,menu[i].name);
        }
 
        //ask for prompt and verify it
        printf("Option: ");
        scanf("%d",&input);
        fgetc(stdin); // clean the stdin from '\n'
        if (input>=0 && input<size){
            printf("Within bounds\n");
            // activate the apropiate function and continue
            menu[input].fun(&my_state);
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