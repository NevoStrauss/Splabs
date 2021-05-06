#include "util.h"

#define _GNU_SOURCE
#define STDOUT 1
#define SYS_EXIT 1
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_LSEEK 19


void switchName(char* fileName, char* name){
  int open =  system_call(SYS_OPEN,fileName,2,0);
  if (open==-1){
      system_call(SYS_EXIT,0,0,0);
  }
  char* buffer[6];
  char* mira = " mira";
  int size = system_call(SYS_LSEEK,open,0,2);
  system_call(SYS_LSEEK,open,0,0);
  int i;
  for (i = 0; i < size; i++){
    int read = system_call(SYS_READ,open,buffer,6);
    if (strncmp("shira ",buffer,6)==0){
        system_call(SYS_WRITE,open,mira,6);
    }
  }
}

int main (int argc , char* argv[], char* envp[]){
  switchName("greeting","nevo");
  system_call(SYS_WRITE,STDOUT,"\nhello world!\n", 14);
  int i;
  char * str;
  str = itoa(argc);
  system_call(SYS_WRITE,STDOUT, str,strlen(str));
  system_call(SYS_WRITE,STDOUT," arguments \n", 12);
  for (i = 0 ; i < argc ; i++){
      system_call(SYS_WRITE,STDOUT,"argv[", 5);
	    str = itoa(i);
      system_call(SYS_WRITE,STDOUT,str,strlen(str));
      system_call(SYS_WRITE,STDOUT,"] = ",4);
      system_call(SYS_WRITE,STDOUT,argv[i],strlen(argv[i]));
      system_call(SYS_WRITE,STDOUT,"\n",1);
  }
  return 0;
}
