#include <linux/limits.h>
#include "LineParser.c"
#define MAX_LEN 2048

int D;

void handleChangeDir(char *path){
    int result = chdir(path);
    if(result != 0)
        perror("Fail to open");
}

void execute(cmdLine *pCmdLine){
    int result;
    pid_t pid = fork();
    if(D==1){
        fprintf(stderr,"PID: %d\n", pid);
        fprintf(stderr,"Executing command: ");
        int j;
        for (j = 0; j < pCmdLine->argCount; j++){
            fprintf(stderr,"%s", pCmdLine->arguments[j]);
        }
    }
    if(!pid){
        /* execute the program, gets path and arguments*/
        /*with execv(), the first argument is a path to the executable.
        With execvp(), the first argument is a filename. It must be converted to a path
        before it can used. This involves looking for the filename in all
        of the directories in the PATH environment variable.*/
        result = execvp(pCmdLine->arguments[0],pCmdLine->arguments);
        if(result == -1){
            freeCmdLines(pCmdLine);
            perror("Failed to execute");
            _exit(1);
        }
    }
    
    pid_t state;
    if(pCmdLine->blocking == 1)
        waitpid(pid,&state,WUNTRACED);
    if(state==-1){
        freeCmdLines(pCmdLine);
        printf("Child failed");
        exit(1);
    }
}

int main(int argc, char **argv){
    int i;
 	for(i=1; i<argc; i++){
		if(strncmp(argv[i],"-D",2)==0){ //regualr debug mode
			D = 1;
			fprintf(stderr,"-D\n");	
        }
	}

    while (1){
        char pathBuff[PATH_MAX];
        getcwd(pathBuff,PATH_MAX);
        printf("%s$ ",pathBuff);
        char strLine[MAX_LEN];
        char *success = fgets(strLine,MAX_LEN,stdin);
        if(success == NULL){
            free(success);
            printf("Couldn't read from stdin\n");
            exit(1);
        }
        if (strncmp(strLine,"quit",4)==0)
            exit(0);

        cmdLine *newLine = parseCmdLines(strLine);
        if(strncmp(newLine->arguments[0],"cd",2)==0)
            handleChangeDir(newLine->arguments[1]);
        else
            execute(newLine);
        freeCmdLines(newLine); 
    }
    return 0;
}