#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>


int main(int argc, char** argv){
    char* ls[] = {"ls","-l",0};
    char* tail[] = {"tail","-n","1",0};
    int fd[2];
    pid_t firstChild;
    pid_t secondChild;

    int my_pipe = pipe(fd);  //step 1 create a pipe
    if (my_pipe==-1){
        perror("pipe creation failed");
        exit(1);
    }
    
    bool D = false;
    int i;
    for(i=1; i < argc; i++){
        if(strncmp("-d",argv[i],2)==0){
            D = true;
        }
    }

    if(D)
        fprintf(stderr, "(parent proccess > forking...)\n");

    firstChild = fork();   //step 2 fork child 1

    //on parent proccess:
    if(firstChild > 0){    
        if(D){
            fprintf(stderr, "(parent proccess > created proccess with id: %d)\n",firstChild);
            fprintf(stderr, "(parent proccess > closing the write end of the pipe)\n");
        }

        close(fd[1]);   //close write end of the pipe

        if(D)
            fprintf(stderr, "(parent proccess > forking...)\n");

        secondChild = fork();

        //parent proccess
        if(secondChild > 0){
            if(D){
                fprintf(stderr, "(parent proccess > created proccess with id: %d)\n",secondChild);
                fprintf(stderr, "(parent proccess > closing the read end of the pipe)\n");
            }

            close(fd[0]);   //close the read end of the pipe

            if(D)
                fprintf(stderr,"(parent proccess > waitin for first child proccess to terminate)\n");
            waitpid(firstChild,NULL,0);

            if(D)
                fprintf(stderr,"(parent proccess > waitin for second child proccess to terminate)\n");
            waitpid(secondChild,NULL,0);

            if(D)
                fprintf(stderr,"(parent_process > exiting…)\n");
            exit(0);
        }

        //on child 2 process:
        else if(secondChild == 0){ 
            if(D)
                fprintf(stderr,"(child2>redirecting stdin to the read end of the pipe…)\n");
            close(0);   //closing stdin

            if(dup(fd[0])==-1){  //try to duplicate the read end to stdin
                perror("duplicate failed");
                exit(1);
            }

            close(fd[0]);     //close the fd that was duplicated

            if(D)
                fprintf(stderr,"(child2>going to execute cmd: tail -n 2)\n");

            execvp(tail[0],tail); //execute tail -n 2

            // fprintf(stderr, "failed to execute tail\n");
            // _exit(1);
        }

        else{
            perror("fork failed in the second child\n");
            exit(1);
        }
    }
    
    //on child 1 process:
    else if (firstChild == 0){ 
        if(D)
            fprintf(stderr,"(child1>redirecting stdout to the write end of the pipe…)\n");
        close(1);            //closing stdout
        if(dup(fd[1])==-1){  //try to duplicate the write end to stdout
            perror("duplicate failed");
            exit(1);
        }   
        close(fd[1]);        //close the file descriptor that was duplicated
        if(D)
            fprintf(stderr,"(child1>going to execute cmd: ls -l)\n");
        execvp(ls[0],ls);    //execute ls -l
        fprintf(stderr, "failed to execute ls\n");
        _exit(1);
    }
    else{
        perror("fork failed\n");
        exit(1);
    }

    return 0;
}