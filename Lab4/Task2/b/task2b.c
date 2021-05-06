#include "util.h"

void printDir(struct dirent* d,int out, char* d_type){
  /*print the directory struct*/
  system_call(SYS_WRITE,out,d->name,strlen(d->name));
  system_call(SYS_WRITE,out,",  data type: ",14);
  system_call(SYS_WRITE,out,d_type,strlen(d_type));
  system_call(SYS_WRITE,out,"\n",1);
}

int main (int argc , char* argv[], char* envp[]){
  int fd, read,i,P=0;
  char d_type;
  char buf[BUF_SIZE];
  char* prefix = "*";
  struct dirent *d;

  for (i=1; i < argc; i++){
    if(strncmp("-p",argv[i],2)==0){
      prefix = argv[i]+2;
      P=1;
    }
  }
  
  fd = system_call(SYS_OPEN,".",O_RONLY,0777);/*open my dir*/
  if (fd < 0)
    system_call(SYS_EXIT, 0x55);
  
  read = system_call(SYS_GETDENTS,fd,buf,BUF_SIZE);
  if (read == -1)
    system_call(SYS_EXIT, 0x55);

  int pos=0;
  while(pos < read) {
    d = (dirent *) (buf + pos);
    d_type = *(buf + pos + d->len - 1);
    char *d_type_string = (d_type == DT_REG) ?  "regular" :
                      (d_type == DT_DIR) ?  "directory" :
                      (d_type == DT_FIFO) ? "FIFO" :
                      (d_type == DT_SOCK) ? "socket" :
                      (d_type == DT_LNK) ?  "symlink" :
                      (d_type == DT_BLK) ?  "block dev" :
                      (d_type == DT_CHR) ?  "char dev" : "???";

    int cmp = strncmp(d->name ,prefix,strlen(prefix));
    if(P==1 && cmp==0)
        printDir(d,STDOUT,d_type_string);
    else if(P!=1)
        printDir(d,STDOUT,d_type_string);
    pos += d->len;  
  }
  system_call(SYS_CLOSE,".");
  return 0;
}






