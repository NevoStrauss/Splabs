#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void regPrint(){
    unsigned int read;
    while ((read = fgetc(stdin)) != EOF){
            if (read>='A' && read<='Z')
                read = read+32; // also possible: read += 'a'-'A'
            printf("%c",read);
    }
}

int main(int argc, char **argv){
    int debugMode = 0;
	for(int i=1; i < argc; i++){
		if(strncmp(argv[i],"-D",2)==0){
			debugMode = 1;
			fprintf(stderr,"-D\n");
		}
        else{
			printf("invalid input argument: %s\n", argv[i]);
			return 1;
		}
	}
    unsigned int read;
    int counter=0;
    if (debugMode!=1)
        regPrint();
    else{
        while((read = fgetc(stdin)) != EOF){
			if(debugMode == 1){
				if(read!='\n'){ 
					fprintf(stderr,"%i  ",read);
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
	return 0;
}
    

