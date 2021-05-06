#include "util.h"
/*#include "/usr/include/dirent.h"*/

void printDir(struct dirent* d,int out){
  /*print the directory struct*/
  system_call(SYS_WRITE,out,d->name,strlen(d->name));
  system_call(SYS_WRITE,out,"\n",1);
}

int main (int argc , char* argv[], char* envp[]){
  int fd, read;
  char buf[BUF_SIZE];
  struct dirent *d;

  fd = system_call(SYS_OPEN,".",O_RONLY,0777); /*open my dir*/
  if (fd < 0)
    system_call(SYS_EXIT, 0x55);
  
  read = system_call(SYS_GETDENTS,fd,buf,BUF_SIZE);
  if (read == -1)
    system_call(SYS_EXIT, 0x55);

  int pos=0;
  while(pos < read) {
    d = (struct dirent *) (buf + pos); /*ask avi why it works*/
    printDir(d,STDOUT);
    pos += d->len;  
  }
  system_call(SYS_CLOSE,".");
  return 0;
}






