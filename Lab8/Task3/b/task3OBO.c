#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "elf.h"
#include <sys/mman.h>

typedef struct fun_desc {
  char *name;
  void (*fun)();
}fun_desc;

struct stat fd_stat;
char debug = 0;
int currentFD = -1;
Elf32_Ehdr* header = NULL;
void* map_start = NULL;

void toggle_debug_mode(){
    if(debug){
        debug = 0;
        printf("Debug flag now off\n");
    }
    else{
        debug = 1;
        printf("Debug flag now on\n");
    }
}

void examine_elf_file(){
    int fd;
    printf("Enter ELF file to examine:\n");
    char buffer[100];
    char file_name[100];
    fgets(buffer, 100, stdin);
    sscanf(buffer,"%s",file_name);
    if(currentFD >= 0){
        if(close(currentFD) == -1){
            printf("couldn't close previous file\n");
            exit(1);
        }
        currentFD = -1;
    }
    if((fd = open(file_name, O_RDONLY)) < 0 ) {
      perror("error in open");
      return;
    }
    if(fstat(fd, &fd_stat) != 0 ) {
      perror("stat failed");
      close(fd);
      return;
    }
    map_start = mmap(0, fd_stat.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (map_start == MAP_FAILED) {
      perror("mmap failed");
      close(fd);
      return;
    }
    if(strncmp((char*) map_start+1,"ELF",3)!=0){
        printf("Not an ELF file!\n");
        munmap(map_start, fd_stat.st_size);
        return;
    }
    header = (Elf32_Ehdr *) map_start;

    currentFD = fd;
    printf("1) Magic bytes 1,2,3 are: %d %d %d\n",
                            header->e_ident[EI_MAG1],header->e_ident[EI_MAG2],header->e_ident[EI_MAG3]);
    printf("2) Data encoding type is: %d\n",header->e_ident[EI_DATA]);
    printf("3) Entry point is: %x\n",header->e_entry);
    printf("4) The file offset in which the section header table resides is: %d\n",header->e_shoff);
    printf("5) The number of section header entries is: %d\n", header->e_shnum);
    printf("6) The size of each section header entry is: %d\n", header->e_shentsize);
    printf("7) The file offset in which the program header table resides is: %d\n",header->e_phoff);
    printf("8) The number of program header entries is: %d\n",header->e_phnum);
    printf("9) The size of each program header entry is: %d\n",header->e_phentsize);
}

void print_section_names(){
    if (header == NULL){
        perror("no file was found\n");
        if(currentFD >= 0){
            close(currentFD);
            currentFD = -1;
        }
        return;
    }
    printf("[index]\tsection_name\tsection_address\tsection_offset\tsection_size\tsection_type");
    if(debug){
        printf("\tsection name offset");
    }
    printf("\n");

    Elf32_Shdr* section_header = map_start + header->e_shoff;
    Elf32_Shdr* section_header_string_table = section_header + header->e_shstrndx;
    char* string_table = map_start + section_header_string_table->sh_offset;
    int i;
    for (i = 0; i < header->e_shnum; i++){
        printf("[%d] %-15s\t%-5.8x\t%-5.6x\t\t%x\t\t%d",i,(char*) string_table + section_header[i].sh_name,
                section_header[i].sh_addr,section_header[i].sh_offset,section_header[i].sh_size,section_header[i].sh_type);
        if (debug){
        printf("\t%d",section_header[i].sh_name);
        }
        printf("\n");
    }
}

Elf32_Shdr search_in_section_header_table(int type){
    Elf32_Shdr* section_header = map_start + header->e_shoff;
    int i;
    for(i = 0; i < header->e_shnum; i++){
        if(type == section_header[i].sh_type){
            return section_header[i];
        }
    }
    return section_header[i];
}

int get_section_size(char* section){
    Elf32_Shdr* section_header = map_start + header->e_shoff;
    Elf32_Shdr* section_header_string_table = section_header + header->e_shstrndx;
    char* string_table = map_start + section_header_string_table->sh_offset;
    int i;
    for(i = 0; i < header->e_shnum; i++){
        if(strncmp(section,(char*) string_table + section_header[i].sh_name,strlen(section)) == 0){
            return section_header[i].sh_size;
        }
    }
    return -1;
}

void print_symbols(){
    if (header == NULL){
        perror("no file was found\n");
        if(currentFD >= 0){
            close(currentFD);
            currentFD = -1;
        }
        return;
    }
    printf("[index]\tvalue");
    if(debug){
        printf("\t\tsize");
    }
    printf("\t\tsection_index\tsection_name\tsymbol_name");
    printf("\n");
    Elf32_Shdr sym_tab_in_section_header = search_in_section_header_table(SHT_SYMTAB);
    Elf32_Sym* symbol_table = (Elf32_Sym*) (map_start + sym_tab_in_section_header.sh_offset);
    if(symbol_table == NULL){
        perror("symbol table wasn't found\n");
        return;
    }
    
    Elf32_Shdr* section_header = map_start + header->e_shoff;
    Elf32_Shdr* section_header_string_table = section_header + header->e_shstrndx;
    char* string_table = map_start + section_header_string_table->sh_offset;
    int sym_size = sym_tab_in_section_header.sh_size / sizeof(Elf32_Sym);
    Elf32_Shdr strtab = section_header[sym_tab_in_section_header.sh_link];
    char* symbol_string_table = (char*) (map_start + strtab.sh_offset);
    int i;
    for (i = 0; i < sym_size; i++){
        printf("[%d]\t%-5.8x", i, symbol_table[i].st_value);
        if (debug){
            printf("\t%.8d",symbol_table[i].st_size);
        }
        if(symbol_table[i].st_shndx == SHN_UNDEF){
            printf("\t%s\t\t%s\t\t%s", "UND",
                                        "",
                                        (char*) symbol_string_table + symbol_table[i].st_name);
            
        }else if (symbol_table[i].st_shndx == SHN_ABS){
            printf("\t%s\t\t%s\t\t%s","ABS",
                                        "",
                                        (char*) symbol_string_table + symbol_table[i].st_name);
        }
        else{
            printf("\t%d\t\t%s\t\t%s", symbol_table[i].st_shndx,
                                        string_table + section_header[symbol_table[i].st_shndx].sh_name,
                                        (char*) symbol_string_table + symbol_table[i].st_name);
        }
        printf("\n");
    }
}


void relocation_tables(){
    Elf32_Shdr* section_header = map_start + header->e_shoff;
    Elf32_Shdr* section_header_string_table = section_header + header->e_shstrndx;
    char* string_table = map_start + section_header_string_table->sh_offset;
    Elf32_Shdr dynamic_in_section_header = search_in_section_header_table(SHT_DYNSYM);
    Elf32_Sym* dynamic_symbol_table = (Elf32_Sym*) (map_start + dynamic_in_section_header.sh_offset);
    Elf32_Shdr str_table = section_header[dynamic_in_section_header.sh_link];
    
    //1.causes segmentation fault or mal-info
    char* dyn_string_table = (char*) (map_start + str_table.sh_offset);
     
    Elf32_Rel* relocation_table;
    Elf32_Shdr* associated_sym_table;
    int i;
    for(i=0; i < header->e_shnum; i++){
        if(section_header[i].sh_type == SHT_REL){
            relocation_table = map_start + section_header[i].sh_offset;
            associated_sym_table = map_start + section_header[i].sh_link;
            char* rel_sec_name = string_table+section_header[i].sh_name;
            
            int rel_size = get_section_size(rel_sec_name) / sizeof(Elf32_Rel);
            int j;
            printf("Relocation section '%s' at offset 0x%x contains %d entries:\n",rel_sec_name,section_header[i].sh_offset,rel_size);
            printf(" Offset     Info    Type            Sym.Value  Sym.Name\n");
            for(j=0; j < rel_size; j++){
                if(*(dyn_string_table+dynamic_symbol_table[ELF32_R_SYM(relocation_table[j].r_info)].st_name)==0){
                    printf("%.8x %.8x    %d\n",
                            relocation_table[j].r_offset,
                            relocation_table[j].r_info,
                            ELF32_R_TYPE(relocation_table[j].r_info));
                }
                else{
                    printf("%.8x %.8x    %d              %.8x   %s\n",
                            relocation_table[j].r_offset,
                            relocation_table[j].r_info,
                            ELF32_R_TYPE(relocation_table[j].r_info),
                            dynamic_symbol_table[ELF32_R_SYM(relocation_table[j].r_info)].st_value,
                            dyn_string_table+dynamic_symbol_table[ELF32_R_SYM(relocation_table[j].r_info)].st_name);
                }
            }
        }
    }
    
}

void quit(){
    if(debug){
        printf("quitting...\n");
    }
    if(currentFD != -1){
        close(currentFD);
        currentFD = -1;
    }
    if(map_start != NULL){
        munmap(map_start, fd_stat.st_size);
        map_start = NULL;
    }
    exit(0);
}


int main(int argc, char **argv){
	fun_desc menu[] ={{"Toggle Debug Mode", toggle_debug_mode},{"Examine ELF File",examine_elf_file},{"Print Section Names",print_section_names},
                            {"Print Symbols",print_symbols},{"Relocation Tables",relocation_tables},{"Quit",quit},{NULL,NULL}};
	char buff[3];
	int index;
	int size = sizeof(menu)/sizeof(menu[0]);
	while(1){
		printf("Choose action:\n");
		for(int i=0; i<size ;i++){
			if(menu[i].name!=NULL)
			printf("%d-%s\n",i,menu[i].name);
		}
		fgets(buff,3,stdin);
		index = atoi(buff);
		if(index < 0 || index > size){
			printf("Not within bounds\n");
			exit(1);
		}
		printf("Within bounds\n");
        menu[index].fun();
	}
	return 0;
}
