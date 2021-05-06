#include "util.h"

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define SYS_EXIT 1
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_LSEEK 19

int main (int argc , char* argv[], char* envp[]){
    char buffer[1024];
    while (1){
        int read = system_call(SYS_READ,STDIN,buffer,1024);
        if (read<0)
            system_call(SYS_EXIT,0x55);
        if (read == 0)
            break;
        int i;
        for (i = 0; i < read; i++){
            if (buffer[i]>='A' && buffer[i]<='Z')
                buffer[i] = buffer[i]+32;
        }
        system_call(SYS_WRITE,STDOUT,buffer,read);
    }
    return 0;
}