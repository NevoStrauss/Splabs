#include "LineParser.c"
#include <linux/limits.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <sys/types.h>
#include <stdbool.h>

#define MAX_LENGTH 2048

bool D = false;

void execute(cmdLine* pCmdLine){
    pid_t pid = fork();
    //debug prints
    if (D){
        fprintf(stderr,"PID: %d\n",pid);
        fprintf(stderr,"Executing Command: ");
        int i;
        for (i = 0; i < pCmdLine->argCount; i++){
            fprintf(stderr," %s", pCmdLine->arguments[i]);
        }
        fprintf(stderr,"\n");
    }
    
    //child process
    if (pid == 0){
        //if redirection input:
        //close stdin and open a new file that will get the file descriptor of stdin
        if(pCmdLine->inputRedirect != NULL){    
            close(0);
            fopen(pCmdLine->inputRedirect,"r");
        }
        //if redirection output:
        //close stdout and open a new file that will get the file descriptor of stdout
        if(pCmdLine->outputRedirect !=NULL){    
            close(1);
            fopen(pCmdLine->outputRedirect,"w");
        }

        //execute command
        int execute = execvp(pCmdLine->arguments[0], pCmdLine->arguments);

        if(pCmdLine->inputRedirect != NULL){
            fclose((FILE*)pCmdLine->inputRedirect);  //the new input file we opened
        }

        if(pCmdLine->outputRedirect !=NULL){
            fclose((FILE*)pCmdLine->outputRedirect);  //the new output file we opened
        }

        if (execute == -1){
            perror("Error executing the command\n");
            freeCmdLines(pCmdLine);
            _exit(1);
        }
    }

    //parent process
    else if (pid > 0){
        int status;
        if (pCmdLine->blocking == 1){    //need to wait for child
            waitpid(pid, &status, WUNTRACED); 
        }

        if (status == -1){
            printf("Child finished with failure");
            freeCmdLines(pCmdLine);
            exit(1);
        }
        freeCmdLines(pCmdLine);
    }

    else{
        perror("fork failed");
        exit(1);
    }
}

void executePipe(cmdLine* curr){
    int fd[2];
    int firstChild;
    int secondChild;
    pid_t my_pipe = pipe(fd);
    cmdLine* next = curr->next;

    if (my_pipe==-1){
        perror("pipe failed");
        freeCmdLines(curr);
        exit(1);
    }

    if(D)
        fprintf(stderr, "(parent proccess > forking...)\n");
        
    firstChild = fork();

    //parent proccess
    if(firstChild > 0){   
        if(D)
            fprintf(stderr, "(parent proccess > created proccess with id: %d)\n",firstChild);
        close(fd[1]);   //close write end of the pipe

        if(D){
            fprintf(stderr, "(parent proccess > closing the write end of the pipe)\n");
            fprintf(stderr, "(parent proccess > forking...)\n");
        }

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
            
            //meaning wait for any child process whose process group ID
            //is equal to that of the calling process.
            waitpid(firstChild,NULL,0);
            if(D)
                fprintf(stderr,"(parent proccess > waitin for second child proccess to terminate)\n");
            
            waitpid(secondChild,NULL,0);
            
            if(D)
                fprintf(stderr,"(parent_process > exiting…)\n\n");
        }

        //second child
        else if(secondChild == 0){ 
            if(next->inputRedirect != NULL){    //close stdin and open a new file that will get the file descriptor of stdin
                close(0);
                fopen(next->inputRedirect,"r");
            }
            if(next->outputRedirect !=NULL){    //close stdout and open a new file that will get the file descriptor of stdout
                close(1);
                fopen(next->outputRedirect,"w");
            }
            if(D)
                fprintf(stderr,"(child2>redirecting stdin to the read end of the pipe…)\n");
            
            close(0);   //closing stdin -> same asclose(STDIN_FILENO);?

            int readEndDup = dup(fd[0]); //duplicate the read end to stdin

            if(readEndDup==-1){  
                perror("duplicate failed");
                freeCmdLines(curr);
                exit(1);
            }

            close(fd[0]);     //close the fd that was duplicated, (fclose is for FILE)

            if(D)
                fprintf(stderr,"(child2>going to execute cmd: tail -n 2)\n");

            execvp(next->arguments[0],next->arguments);

            if(next->inputRedirect != NULL)
                fclose((FILE*)next->inputRedirect);  //the file we opened
            
            if(next->outputRedirect !=NULL)
                fclose((FILE*)next->outputRedirect);  //the file we opened
            
            freeCmdLines(curr);
            _exit(1);
        }
        //end second child
        
        else{
            perror("fork failed in the second child\n");
            freeCmdLines(curr);
            exit(1);
        }
    }
    //end parent
    
    //first child
    else if (firstChild == 0){
        if(curr->inputRedirect != NULL){    //close stdin and open a new file that will get the file descriptor of stdin
            close(0);
            fopen(curr->inputRedirect,"r");
        }

        if(curr->outputRedirect !=NULL){    //close stdout and open a new file that will get the file descriptor of stdout
            close(1);
            fopen(curr->outputRedirect,"w");
        }

        if(D)
            fprintf(stderr,"(child1>redirecting stdout to the write end of the pipe…)\n");
        close(1);  //closing stdout

        int writEndDup = dup(fd[1]);

        if(writEndDup==-1){  //duplicate the write end to stdout
            perror("duplicate failed");
            exit(1);
        }

        close(fd[1]);    //close the filedescriptor that was duplicated

        if(D)
            fprintf(stderr,"(child1>going to execute cmd: ls -l)\n");

        execvp(curr->arguments[0],curr->arguments);
        if(curr->inputRedirect != NULL){
            fclose((FILE*)curr->inputRedirect);  //the file we opened
        }
        if(curr->outputRedirect !=NULL){
            fclose((FILE*)curr->outputRedirect);  //the file we opened
        }
        _exit(1);
    }
    //end first child

    else{
        perror("fork failed\n");
        exit(1);
    }
    return;
}

int main(int argc, char** argv){
    int i;
    for (i = 1; i < argc; i++){
        if(strncmp("-d",argv[i],2)==0){
            D = true;
        }
    }

    while(true){   
        char dir_buff[PATH_MAX];
        char command_buff[MAX_LENGTH]; 
        char* curr_dir = getcwd(dir_buff, PATH_MAX);
        if (curr_dir==NULL){
            printf("getcwd went wrong\n");
            exit(1);
        }

        printf("\n%s$ ",dir_buff);
        fflush(stdout);
        char* newLine = fgets(command_buff, MAX_LENGTH, stdin);
        if (newLine == NULL){
            printf("stdin EOF\n");
            exit(1);
        }

        cmdLine* cmdLine = parseCmdLines(command_buff);

        if(cmdLine == NULL){
            printf("Parse failed\n");
            freeCmdLines(cmdLine);
        }

        if(strncmp(cmdLine->arguments[0],"quit",4)==0)
            break;

        else{
            if(cmdLine->next!=NULL){    //then it was with a pipe
                executePipe(cmdLine);
                freeCmdLines(cmdLine);
            }

            else if (strncmp(cmdLine->arguments[0],"cd",2)==0){
                if(chdir(cmdLine->arguments[1])<0){
                    fprintf(stderr, "Can't change directory to %s\n",cmdLine->arguments[1]);
                }
                freeCmdLines(cmdLine);
            }
            else{
                execute(cmdLine);
            }
        }
        
    }
    return 0;
}