#include <linux/limits.h>
#include "LineParser.c"
#define MAX_LEN 2048

void addProcess(process **process_list, cmdLine* cmd, pid_t pid);
void printProcessList(process **process_list);
void freeProcessList(process* process_list);
void updateProcessList(process **process_list);
void updateProcessStatus(process* process_list, int pid, int status);

int D;
process *my_process_list = NULL;

void handleChangeDir(char *path){
    int result = chdir(path);
    if(result != 0)
        perror("Fail to open");
}

void freeProcessList(process *process_list){
    process* curr;
    if(process_list != NULL){
        curr = process_list->next;
        freeCmdLines(process_list->cmd);
        free(process_list);
    }
    if (curr != NULL)
        freeProcessList(curr);
}

void addProcess(process** process_list, cmdLine* cmd, pid_t pid){
    process *newLink = (process*)malloc(sizeof(process));
    newLink->cmd = cmd;
    newLink->pid=pid;
    newLink->status = RUNNING;
    newLink->next = my_process_list;
    my_process_list = newLink;
}

/*auxilery function for print*/
void statusPrint(process* p){
    if(p->status == 1)
        printf("RUNNING"); 
    if(p->status == 0)
        printf("SUSPENDED");
    if(p->status == -1)
        printf("TERMINATED");
}

void printProcessList(process **process_list){
    updateProcessList(process_list);
    process* curr = my_process_list;
    printf("PID         COMMAND         STATUS\n");
    while(curr != NULL){
        printf("%d         %s         ",curr->pid, curr->cmd->arguments[0]);
        statusPrint(curr);
        printf("\n");
        /* if process was terminated - delete it */
        if(curr->status == TERMINATED){
            process* prev = my_process_list;
            
            if(curr == prev){        // if we need to delete the first node
                my_process_list = curr->next;
                freeCmdLines(curr->cmd);
                free(curr);
                curr = my_process_list;
            }
            else{
                /*bring a pointer to the prev of the current node needed to delete*/
                while(prev->next != curr){
                    prev = prev->next;
                }
                prev->next = curr->next; /*set pointers before deleting*/
                freeCmdLines(curr->cmd);
                free(curr);
                curr = prev->next;
            }          
        }
        else
            curr = curr->next;
    }
}

void updateProcessStatus(process* process_list, int pid, int status){
    process* current = my_process_list;
    while(current != NULL){
        /* update the information*/
        if(current->pid == pid){
            current->status = status;
            break;
        }
        /* go to the next proccess*/
        current = current->next;
    }
}

void updateProcessList(process **process_list){
   process* curr = my_process_list;
    int status;
    while(curr != NULL){
        /* update the information*/
        //printf("\status: %d\n pid:%d\n", current->status,current->pid);
        pid_t result = waitpid(curr->pid,&status, WNOHANG | WUNTRACED | WCONTINUED);
        if(curr->pid == result){
            if (WIFEXITED(status)){
                printf("exited, status=%d\n", WEXITSTATUS(status)); 
                updateProcessStatus(my_process_list,curr->pid,TERMINATED); 
            }
            if (WIFSIGNALED(status)) {
                updateProcessStatus(my_process_list,curr->pid,TERMINATED); 
                printf("killed by signal %d\n", WTERMSIG(status));
            }
            else if (WIFSTOPPED(status))
            {
                updateProcessStatus(my_process_list, curr->pid, SUSPENDED);
                printf("stopped by signal %d\n", WSTOPSIG(status));
            }
            else if (WIFCONTINUED(status))
            {
                updateProcessStatus(my_process_list, curr->pid, RUNNING);
                printf("continued\n");
            }   
            printf("%d", status);
        }
        else if(result == -1){
            updateProcessStatus(my_process_list,curr->pid,TERMINATED);
        }
        /* go to the next proccess*/
        curr = curr->next; 
    }
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
    if(pid == 0){ //child created
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
    else if(pid == -1){
        perror("Error forking the child process");
        _exit(1);
    }
    
    pid_t state;
    if(pCmdLine->blocking == 1)
        waitpid(pid,&state,WUNTRACED);
    if(state==-1){
        freeCmdLines(pCmdLine);
        printf("Child failed");
        exit(1);
    }
    addProcess(&my_process_list,pCmdLine,pid);
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

        cmdLine *newLine = parseCmdLines(strLine);
        char *command = newLine->arguments[0];

        if (strncmp(command,"quit",4)==0)
            break;

        if(strncmp(command,"cd",2)==0){
            handleChangeDir(newLine->arguments[1]);
            freeCmdLines(newLine);
        }

        else if(strncmp(command,"procs",5)==0){
            printProcessList(&my_process_list);
            freeCmdLines(newLine);
        }
        else
            execute(newLine);
    }
    freeProcessList(my_process_list);
    return 0;
}