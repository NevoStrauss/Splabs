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

void fromInputFile(int shift, FILE* input){
	unsigned int read;
    while ((read = fgetc(input)) != EOF){
        read = read+shift;
        printf("%c",read);
    }
}
	
void fromStdin(int shift){
	unsigned int read;
    while ((read = fgetc(stdin)) != EOF){
        read = read+shift;
        printf("%c",read);
    }
}

int main(int argc, char **argv){
	FILE *input;
	int debugMode=0, isInputFile=0;
	int shift=0, operator=0;
	for(int i=1; i<argc; i++){
		if(strncmp(argv[i],"-D",2)==0){ //regualr debug mode
			debugMode = 1;
			fprintf(stderr,"-D");	
		}
		else if((argv[i][0]=='-' || argv[i][0]=='+') && argv[i][1]=='e'){ //encryption mode
			operator = (argv[i][0]=='-')?  -1 : 1;
			char c = argv[i][2];
			shift = (c>='0' && c <='9') ? operator*((int)c-'0'): 
                    (c>='A' && c <='F') ? operator*((int)c-'A'+10): 16;
            if(shift == 16){ // -15<16<15
				printf("invalid input argument: %s\n",argv[i]);
				exit(0);
			}
		}

		else if(argv[i][0]=='-' && argv[i][1]=='i'){ //input file mode
			isInputFile =1;
			input = fopen(argv[i]+2,"r");
			if (input==NULL){
				printf("Can't open the file");
				exit(0);	
			}
		}

		else{ //invalid input
			printf("invalid input argument: %s\n",argv[i]);
			exit(0);
		}
	}
	
	//start proccesing:	
	fprintf(stderr,"\n");
	if (operator==0 && isInputFile==0) //regular
		unEncrypted(debugMode);
	else if (isInputFile==1) //need to read from input file
		fromInputFile(shift, input);
	else                  //need to read from Stdin
		fromStdin(shift);
	return 0;
}
	


