#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void handleSignal(int sig){
	printf("Recived signal: %s\n signal number: %d\n", strsignal(sig),sig);
	if(sig==SIGTSTP)
		signal(SIGCONT, handleSignal);
	else if(sig == SIGCONT)
		signal(SIGTSTP, handleSignal);
	signal(sig, SIG_DFL);
	raise(sig);
}

int main(int argc, char **argv){
	signal(SIGTSTP,handleSignal);
	signal(SIGINT, handleSignal);
	signal(SIGCONT, handleSignal);

	printf("Starting the program\n");
	while(1) {
		sleep(2);
	}
	return 0;
}