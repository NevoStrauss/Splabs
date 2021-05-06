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

void readFromInput(char* input, int isOutputFile, char* output){
    char buffer[1024];
    int in = system_call(SYS_OPEN,input,0,0777);
    int out;
    if (isOutputFile==1){
        out = system_call(SYS_OPEN,output,1|64,0777);
        if (out<0){
            system_call(SYS_WRITE,STDOUT,"open output file error\n",23);
            system_call(SYS_EXIT,0x55);
        }
    }
    while (1){
        int read = system_call(SYS_READ,in,buffer,1024);
        if (read<0){
            system_call(SYS_WRITE,STDOUT,"read input error\n",17);
            system_call(SYS_EXIT,0x55);
        }
        if (read == 0)
            break;
        int i;
        for (i = 0; i < read; i++){
            if (buffer[i]>='A' && buffer[i]<='Z')
                buffer[i] = buffer[i]+32;
        }
        if (isOutputFile==1){
            system_call(SYS_WRITE,out,buffer,read);
            system_call(SYS_CLOSE,out);
        }
        else
            system_call(SYS_WRITE,STDOUT,buffer,read);
    }
    system_call(SYS_CLOSE,in);
}

void readFromSTDIN(int isOutputFile,char* output){
    char buffer[1024];
    int out;
    if (isOutputFile==1){
                        /*command,path,ronly|create,per*/
        out = system_call(SYS_OPEN,output,1|64,0777);
        if (out<0){
            system_call(SYS_WRITE,STDOUT,"open output file error\n",23);
            system_call(SYS_EXIT,0x55);
        }
    }
    while (1){
        int read = system_call(SYS_READ,STDIN,buffer,1024);
        if (read<0){
            system_call(SYS_WRITE,STDOUT,"read stdin error\n",17);
            system_call(SYS_EXIT,0x55);
        }
        if (read == 0)
            break;
        int i;
        for (i = 0; i < read; i++){
            if (buffer[i]>='A' && buffer[i]<='Z')
                buffer[i] = buffer[i]+32;
        }
        if (isOutputFile==1){
            system_call(SYS_WRITE,out,buffer,read);
            system_call(SYS_CLOSE, out);
        }
        else
            system_call(SYS_WRITE,STDOUT,buffer,read);
    }
}

int main (int argc , char** argv, char* envp[]){
    int isInputFile=0,isOutputFile=0;
    char* input, *output;
    int i;
    for(i=1; i<argc; i++){
        if(argv[i][0]=='-' && argv[i][1]=='i'){ /*input file mode*/
			isInputFile=1;
            input=argv[i]+2;
        }
		else if(argv[i][0]=='-' && argv[i][1]=='o'){ /*outputFileMode*/
			isOutputFile=1;
            output=argv[i]+2;
        }
        else
            system_call(SYS_WRITE,STDOUT,"INVALID ARGUMENTS\n", 18);
    }
    if (isInputFile == 1)
        readFromInput(input,isOutputFile,output);
    else
        readFromSTDIN(isOutputFile,output);
    return 0;
}
