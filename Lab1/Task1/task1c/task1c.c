#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void regPrint(){
	unsigned int read;
    while ((read = fgetc(stdin)) != EOF){
        if (read>='A' && read<='Z')
            read = read+32; // also possible: read = 'a'-'A'
		printf("%c",read);
    }
}

void unEncrypted(int debugMode){
    unsigned int read;
	int counter=0;
	if (debugMode != 1)
		regPrint();
	else{
		while((read = fgetc(stdin)) != EOF){
			if(debugMode == 1){
				if(read!='\n'){
					fprintf(stderr,"%i ",read);
					if(read>= 'A' && read <= 'Z'){
						read = read+32; // also possible: read = 'a'-'A'
						counter++;
					}
					fprintf(stderr,"%i\n",read);			
				}
				else{
					fprintf(stderr,"\nthe number of letters: %i\n\n", counter);
					counter = 0;
				}
			}
			fprintf(stdout,"%c", read);
		}
	}
}

void encrypted(int shift){
    unsigned int read;
    while ((read = fgetc(stdin)) != EOF){
        read = read+shift;
        printf("%c",read);
    }
}

int main(int argc, char **argv){
	int operator = 0, shift=0, debugMode=0;
	for(int i=1; i<argc; i++){
		if(strncmp(argv[i],"-D",2)==0){
			debugMode = 1;
			fprintf(stderr,"-D\n");	
		}
	}
	for(int i=1; i < argc && debugMode==0; i++){
		if((argv[i][0] == '-' || argv[i][0] == '+') && argv[i][1] == 'e'){
			operator = (argv[i][0] == '-') ? -1 : 1; //-1 for minus, 1 for plus
			char c = argv[i][2];
			shift = (c>='0' && c <='9') ? operator*((int)c-'0'): 
                    (c>='A' && c <='F') ? operator*((int)c-'A'+10): 16;
            if(shift == 16){ // -15<16<15
				printf("invalid input argument: %s\n",argv[i]);
				exit(0);
			}
		}
        else{
			printf("invalid input argument: %s\n", argv[i]);
            exit(0);
		}
	}
	if(operator==0)
		unEncrypted(debugMode);
	else
		encrypted(shift);
	return 0;
}