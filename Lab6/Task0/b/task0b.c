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
    int result;
    pid_t pid = fork();
    if (D){
        fprintf(stderr,"PID: %d\n",pid);
        fprintf(stderr,"Executing Command:");
        for (size_t i = 0; i < pCmdLine->argCount; i++){
            fprintf(stderr," %s", pCmdLine->arguments[i]);
        }
        fprintf(stderr,"\n");
    }
    
    if (pid == 0){
        if(pCmdLine->inputRedirect != NULL){    //close stdin and open a new file that will get the file descriptor of stdin
            fclose(stdin);
            fopen(pCmdLine->inputRedirect,"r");
        }
        if(pCmdLine->outputRedirect !=NULL){    //close stdout and open a new file that will get the file descriptor of stdout
            fclose(stdout);
            fopen(pCmdLine->outputRedirect,"w");
        }
        result = execvp(pCmdLine->arguments[0], pCmdLine->arguments);
        if(pCmdLine->inputRedirect != NULL){
            fclose((FILE*)pCmdLine->inputRedirect);  //the file we opened
        }
        if(pCmdLine->outputRedirect !=NULL){
            fclose((FILE*)pCmdLine->outputRedirect);  //the file we opened
        }
        if (result == -1){
            perror("Couldn't execute the command\n");
            freeCmdLines(pCmdLine);
            _exit(1);
        }
    }
    else if (pid > 0){
        int status;
        if (pCmdLine->blocking == 1){
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


int main(int argc, char** argv){
    int i;
    for (i = 1; i < argc; i++){
        if(strncmp("-d",argv[i],2)==0){
            D = true;
        }
    }

    while(true){   
        char path[PATH_MAX];
        char buff[MAX_LENGTH]; 
        char* curr_dir = getcwd(path, PATH_MAX);
        if (curr_dir==NULL){
            printf("getcwd went wrong\n");
            exit(1);
        }
        
        printf("%s$ ",path);
        fflush(stdout);

        char* newLine = fgets(buff, MAX_LENGTH, stdin);
        if (newLine == NULL){
            printf("stdin EOF\n");
            exit(1);
        }

        cmdLine* cmdLine = parseCmdLines(buff);

        if(cmdLine == NULL){
            printf("There is nothing to parse\n");
            freeCmdLines(cmdLine);
        }

        if(strncmp(cmdLine->arguments[0],"quit",4)==0)
            break;

        else if (strncmp(cmdLine->arguments[0],"cd",2)==0){
            if(chdir(cmdLine->arguments[1])<0){
                fprintf(stderr, "No such directory as: %s\n",cmdLine->arguments[1]);
            }
            freeCmdLines(cmdLine);
        }
        else
            execute(cmdLine);
    }
    return 0;
}