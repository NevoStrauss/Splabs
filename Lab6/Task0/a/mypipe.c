#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


//in pipe, fd[0] is the read end of the pipe, and fd[1] is the write end of the pipe

//data that is written to the write end is moved to the read end of the pipe

int main(int argc, char** argv){
    int fd[2];  //stands for - file descriptors
    char hello[6];

    int my_pipe = pipe(fd);
    if(my_pipe != 0){
        perror("Pipe go wrong");
        exit(1);
    }
    
    pid_t pid = fork();

    //parent process
    if(pid > 0){                //parent, needs to read from fd[0]
        wait(NULL);             //wait for child proccess
        close(fd[1]);           //first close unused write end
        close(stdin);           //closing the standard input
        read(fd[0],hello,6);    //reading 6 bytes from fd[0] a.k.a the read end
        printf("%s",hello);
        close(fd[0]);
        exit(0);
    }

    else if(pid == 0){  //child, needs to write to fd[1]
        close(fd[0]);   //close standard input
        close(stdout);   //close standard output
        write(fd[1],"hello\n",6);       //write 6 bytes to fd[1] aka the write end
        close(fd[1]);   //close last fd
        _exit(0);
    }
    
    else{
        perror("fork didn't succeed");
        exit(1);
    }
    return 0;
}