#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// index of curr position of file
int currIndex;

int PrintHex(FILE *output, char buffer[], size_t length){
    for (size_t i = 0; i < (length - 1); i++){
        fprintf(output, "%02hhX ", buffer[i]);
    }
    fprintf(output, "%02hhX\n", buffer[length - 1]);
    return 0;
}

typedef struct virus{
    unsigned short sigSize;
    char virusName[16];
    unsigned char *sig;
} virus;

// Return 'L' or 'B' according to Little or Big Endian in the header.
char processHeader(FILE *fp){
    // Make sure file supplied isn't empty.
    if (fp == NULL){
        printf("\nError! supplied file is empty.\n");
        exit(1);
    }

    // Make sure that Virus has an header in the right form.
    // If so, assign the endian as suppplied
    char *header;
    header = (char *)malloc(4);
    char endian;
    fread(header, 1, 4, fp);
    if (header[0] != 'V' || header[1] != 'I' || header[2] != 'S'){
        printf("Error! Virus's Header is incorrect.");
        exit(1);
    }
    if (header[3] == 'L')
        endian = 'L';
    else if (header[3] == 'B')
        endian = 'B';
    else{
        printf("Error! Virus's Header is incorrect - missing small or big endian form..");
        exit(1);
    }

    // currIndex is updated to skip header.
    currIndex += 4;
    //fseek(fp, currIndex, SEEK_SET);
    free(header);

    return endian;
}

short calcLitlleEndian(char left, char right){
    short output = (((short)right) << 8) | left;
    return output;
}

/*
this function receives a file pointer and returns
a virus* that represents the next virus in the file.
*/
virus *readVirus(FILE *fp){
    char sizeAndName[18];
    char virusName[16];

    // Get sig-size and name. Promote the cursor to the right place.
    fread(sizeAndName, 1, 18, fp);
    currIndex += 18;
    fseek(fp, currIndex, SEEK_SET);

    unsigned short sigSize = calcLitlleEndian(sizeAndName[0], sizeAndName[1]);

    memcpy(virusName, sizeAndName + 2, 16);
    // Get signature itself
    char *sig = (char *)malloc(sigSize); // we'll know the size only dynamically
    fread(sig, 1, sigSize, fp);
    currIndex += sigSize;
    fseek(fp, currIndex, SEEK_SET);

    virus *vir;
    vir = malloc(sizeof(virus)); //mallocs 24 bytes
    vir->sigSize = sigSize;
    memcpy((vir->virusName), virusName, 16);
    vir->sig = sig;
    // how to free virus?
    return vir;
}

/*
this function receives a pointer to a virus
and a pointer to an output file.
The function prints the virus to
the given output file.
It prints the virus name (in ASCII),
the virus signature length (in decimal),
and the virus signature (in hexadecimal representation). 
*/
void printVirus(virus *virus, FILE *output){
    fprintf(output, "Virus name: %s\n", virus->virusName);
    fprintf(output, "Virus size: %d\n", virus->sigSize);
    fprintf(output, "signature:\n");
    PrintHex(output, virus->sig, virus->sigSize);
}

int main(int argc, char const *argv[]){
    FILE *fp;
    fp = fopen("./signatures-L", "r");
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    processHeader(fp);
    // Get next viruses
    while (currIndex != size){
        virus *vir0 = readVirus(fp);
        printVirus(vir0, stdout);
        printf("\n");
        free(vir0);
    }
    fclose(fp);
    return 0;
}