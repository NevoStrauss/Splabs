#include <linux/limits.h>
#include "LineParser.c"
#define MAX_LEN 2048

int D;

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
        fprintf(stderr,"\n");
    }
    if(!pid){
        /* execute the program, gets path and arguments*/
        /*with execv(), the first argument is a path to the executable.
        With execvp(), the first argument is a filename. It must be converted to a path
        before it can used. This involves looking for the filename in all
        of the directories in the PATH environment variable.*/
        result = execvp(pCmdLine->arguments[0],pCmdLine->arguments);
        if(result == -1){
            perror("Failed to execute");
            _exit(1);
        }
    
        pid_t state;
        waitpid(pid,&state,WUNTRACED);
        if(state==-1){
            printf("Child failed");
            exit(1);
        }
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
            printf("Couldn't read from stdin\n");
            exit(1);
        }
        if (strncmp(strLine,"quit",4)==0)
            exit(0);
        cmdLine *newLine = parseCmdLines(strLine);
        execute(newLine);
        freeCmdLines(newLine); 
    }
    return 0;
}