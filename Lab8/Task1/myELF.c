#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>
#include <sys/mman.h>


typedef struct fun_desc{
  char *name;
  void (*fun)();
}fun_desc;

Elf32_Ehdr *header;
struct stat fd_stat;
void *map_start;

int fd = -1;
bool debug = false;

void debug_print_members(){

}

void Toggle_Debug_Mode(){
    if(!debug){
        debug = true;
        printf("%s\n","Debug flag now on");
    }
    else if(debug){
        debug = false;
        printf("%s\n","Debug flag now off");
    }
}

void print_info(){
   printf("\nThe ASCII value of Bytes 1,2,3 of the magic: %d %d %d\n", header->e_ident[EI_MAG1], header->e_ident[EI_MAG2], header->e_ident[EI_MAG3]);
   printf("The data encoding scheme of the object file: %d\n", header->e_ident[EI_DATA]);
   printf("The Hexadecimal address of the Entry point: %x\n", header->e_entry);
   printf("The file offset in which the sec header table resides: %d\n", header->e_shoff);
   printf("The number of sec header entries: %d\n", header->e_shnum);
   printf("The size of each sec header entry: %d\n", header->e_shentsize);
   printf("the file offset in which the program header table resides: %d\n", header->e_phoff);
   printf("the number of program header entries: %d\n", header->e_phnum);
   printf("the size of each progaram header entry: %d\n", header->e_phentsize);

}

void  reset_fd(){
    close(fd);
    fd = -1;
}

void Examine_ELF_File(){
    printf("Please enter new ELF file name: ");
    char buff[100];
    char elfFile[100];
    fgets(buff,100,stdin);
    sscanf(buff, "%s", elfFile);
    if(debug)
        printf("print something...");

    if (fd > -1){
        if(close(fd)==-1){
            perror("Cannot close the file");
            exit(1);
        }
    }

    if((fd = open(elfFile, O_RDWR)) < 0 ) {
      perror("error in open");
      exit(-1);
    }

   if(fstat(fd, &fd_stat) != 0 ) {
      reset_fd();
      perror("stat failed");
      exit(-1);
   }

   if ((map_start = mmap(0, fd_stat.st_size, PROT_READ | PROT_WRITE , MAP_SHARED, fd, 0)) == MAP_FAILED ) {
      reset_fd();
      perror("mmap failed");
      exit(-4);
   }

   /* now, the file is mapped starting at map_start.
    * all we need to do is tell *header to point at the same address:
    */

   /* now we can do whatever we want with header!!!!
    * for example, the number of section header can be obtained like this:
    */
   if(strncmp((char*)map_start+1,"ELF",3) != 0){
       printf("%s %s",elfFile," is not ELF file");
       reset_fd();
       return;
   }

   header = (Elf32_Ehdr *) map_start;

   print_info();

}

char* typeToString(int numtype){
    switch (numtype)
    {
    case 0x06: return "SHT_DYNAMIC";
        break;

    case 0xb: return "SHT_DYNSYM";
        break;

    case 0xf: return "SHT_FINI_ARRAY";
        break;

    case 0x5: return "SHT_HASH";
        break;
    
    case 0x7fffffff: return "SHT_HIPROC";
        break;
    
    case 0xffffffff: return "SHT_HIUSER";
        break;

    case 0xe: return "SHT_INIT_ARRAY";
        break;
    
    case 0x70000000: return "SHT_LOPROC";
        break;
    
    case 0x80000000: return "SHT_LOUSER";
        break;
    
    case 0x8: return "SHT_NOBITS";
        break;
    
    case 0x7: return "SHT_NOTE";
        break;

    case 0x0: return "SHT_NULL";
        break;
    
    case 0x10: return "SHT_PREINIT_ARRAY";
        break;

    case 0x1: return "SHT_PROGBITS";
        break;
    
    case 0x9: return "SHT_REL";
        break;

    case 0x4: return "SHT_RELA";
        break;
    
    case 0xa: return "SHT_SHLIB";
        break;

    case 0x3: return "SHT_STRTAB";
        break;
    
    case 0x2: return "SHT_SYMTAB";
        break;

    default: return "Unknown type";
        break;
    }
}

void Print_Section_Names(){
    if(header == NULL){
        printf("No file loaded\n");
        return;
    }

    if(fd>-1)
        reset_fd();
    
    // get the section header table pointer
    Elf32_Shdr *sec_header_ptr = (Elf32_Shdr *)(map_start + header->e_shoff);

    //in section header, get the string table raw
    Elf32_Shdr *secNames = sec_header_ptr+header->e_shstrndx;

    //get the section headers string table
    char * sec_header_str_table = map_start + secNames->sh_offset;

    // get the number of headers we need to read
    int num_of_sections = header->e_shnum;

    printf("index\tsection_name\tsection_address\tsection_offset\tsection_size\tsection_type");
    if(debug){
        printf("\tname offset");
    }
    printf("\n");
    for (int i = 0; i < num_of_sections; i++){
        printf("[%d] %-20s\t%-5.6x\t%7x\t\t%7x\t\t%s",
        i,
        sec_header_str_table+sec_header_ptr[i].sh_name,
        sec_header_ptr[i].sh_addr,
        sec_header_ptr[i].sh_offset,
        sec_header_ptr[i].sh_size,
        typeToString(sec_header_ptr[i].sh_type));

        if(debug)
            printf("%d",sec_header_ptr[i].sh_name);
        
        printf("\n");
    }
}

void Print_Symbols(){

}

void Relocation_Tables(){

}

void Quit(){
    if(debug)
        printf("\n%s\n","quitting..");
    reset_fd();
    munmap(map_start, fd_stat.st_size);
    exit(0);
}

int main(int argc, char **argv){
    fun_desc menu[] = {{"Toggle Debug Mode", Toggle_Debug_Mode},
                       {"Examine_ELF_File", Examine_ELF_File},
                       {"Print Section Names", Print_Section_Names},
                       {"Print Symbols", Print_Symbols},
                       {"Relocation Tables", Relocation_Tables},
                       {"Quit", Quit},
                       {NULL, NULL}};
    

    int input;
    header = NULL;
    int size = sizeof(menu)/sizeof(menu[0]);
    while (true){
        //if debug mode, print the members:
        if(debug)
            debug_print_members();

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
            menu[input].fun(header);
        }

        else{
            printf("Not within bounds\n");
            exit(0);
        }

        printf("DONE.\n\n");
    }
    return 0;
}