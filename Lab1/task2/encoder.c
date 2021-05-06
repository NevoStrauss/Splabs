#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv){
	FILE* input; FILE* output;
	int debugMode=0, isInputFile=0, isOuputFile=0;
	int shift=0, operator=0;
	//check mode by parsing argv
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
            if(shift == 16){ // !-15<16<15
				printf("invalid input argument: %s\n",argv[i]);
				exit(0);
			}
		}

		else if(argv[i][0]=='-' && argv[i][1]=='i'){ //input file mode
			isInputFile=1;
			input = fopen(argv[i]+2,"r");
			if (input==NULL){
				printf("Can't open the file");
				exit(0);	
			}
		}

		else if(argv[i][0]=='-' && argv[i][1]=='o'){ //outputFileMode
			isOuputFile=1;
			fprintf(stderr, "-o ");
			output = fopen(argv[i]+2,"w");
		}

		else{ //invalid input
			printf("invalid parameter - %s\n",argv[i]);
			exit(0);
		}
	}

	//start processing:
	fprintf(stderr,"\n");
	unsigned int read;
	int counter=0;
	while(1){
		read = (isInputFile==1)? fgetc(input) : fgetc(stdin);			
		if(read==EOF)
			break;
		if(read!='\n'){
			if(debugMode==1)
				fprintf(stderr,"%i",read); //print first time for debug mode
			if(operator!=0 && shift!=0){   //encrypt mode, handle shift & increase counter
				read=read+shift;
				counter++;
			}
			else if(operator==0 && read>='A' && read<='Z'){ 
				read = read+32;
				counter++;
			}
			if(debugMode==1)
				fprintf(stderr,"  %i\n",read);
		}
		else if(debugMode==1) {
			fprintf(stderr, "the number of letters: %i\n", counter);
		}

		if (isOuputFile!=1)		
			printf("%c",read);
		else
			fputc(read,output);
		}	
	printf("%s","\n");		
	return 0;
}



