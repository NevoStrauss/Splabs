#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int currIndex;

int PrintHex(FILE *output, char buffer[], size_t length)
{
    for (size_t i = 0; i < (length - 1); i++)
    {
        fprintf(output, "%02hhX ", buffer[i]);
    }
    fprintf(output, "%02hhX\n", buffer[length - 1]);
    return 0;
}

typedef struct virus
{
    unsigned short sigSize;
    char virusName[16];
    unsigned char *sig;
} virus;

typedef struct link link;

struct link
{
    link *nextVirus;
    virus *vir;
};

char processHeader(FILE *fp)
{
    // Make sure file supplied isn't empty.
    if (fp == NULL)
    {
        printf("\nError! supplied file is empty.\n");
        exit(1);
    }

    // Make sure that Virus has an header in the right form.
    // If so, assign the endian as suppplied
    char *header;
    header = (char *)malloc(4);
    char endian;

    fread(header, 1, 4, fp);
    if (header[0] != 'V' || header[1] != 'I' || header[2] != 'S')
    {
        printf("Error! Virus's Header is incorrect.");
        exit(1);
    }
    if (header[3] == 'L')
    {
        endian = 'L';
    }
    else if (header[3] == 'B')
    {
        endian = 'B';
    }
    else
    {
        printf("Error! Virus's Header is incorrect - missing small or big endian form..");
        exit(1);
    }
    currIndex += 4;
    fseek(fp, currIndex, SEEK_SET);

    free(header);
    return endian;
}

short calcLitlleEndian(char left, char right)
{
    short output = (((short)right) << 8) | left;
    return output;
}

/*
this function receives a file pointer and returns
a virus* that represents the next virus in the file.
*/
virus *readVirus(FILE *fp)
{
    if (currIndex == 0)
    {
        processHeader(fp);
    }
    char sizeAndName[18];
    char virusName[16];

    // Get sig-size and name. Promote the cursor to the right place.
    unsigned short numOfSigns;

    // printf("peled idea: %d\n", numOfSigns);
    fread(sizeAndName, 1, 18, fp);
    currIndex += 18;
    fseek(fp, currIndex, SEEK_SET);

    unsigned short sigSize = calcLitlleEndian(sizeAndName[0], sizeAndName[1]);

    if (sigSize > 301)
    {
        printf("this is sig size: %d\n", sigSize);
        printf("this is left %d\n", sizeAndName[0]);
        printf("this is right %d\n", sizeAndName[1]);

        exit(1);
    }
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
void printVirus(virus *virus, FILE *output)
{
    fprintf(output, "Virus name: %s\n", virus->virusName);
    fprintf(output, "Virus size: %d\n", virus->sigSize);
    fprintf(output, "signature:\n");
    PrintHex(output, virus->sig, virus->sigSize);
}

/* Print the data of every link in list to the given stream.
Each item followed by a newline character. */
link *list_print(link *virus_list, FILE *out)
{
    link *currLink = virus_list;
    if (currLink == NULL || currLink->vir == NULL)
    {
        printf("its empty\n");
    }
    while (currLink->nextVirus != NULL)
    {
        printVirus(currLink->vir, out);

        currLink = currLink->nextVirus;
        printf("\n");
    }
    printVirus(currLink->vir, out);
    printf("\n");
    return virus_list;
}
/* Add a new link with the given data to the list 
(either at the end or the beginning, depending on what your TA tells you),
and return a pointer to the list (i.e., the first link in the list).
If the list is null - create a new entry and return a pointer to the entry. */
// Recursive
link *list_append(link *virus_list, virus *data)
{
    // brand new list
    if (virus_list == NULL || virus_list->vir == NULL)
    {
        link *new_link = malloc(sizeof(link));
        new_link->vir = data;
        new_link->nextVirus = NULL;
        return new_link;
    }
    link *curr_link = virus_list;
    link *new_link = malloc(sizeof(link));
    new_link->vir = data;
    new_link->nextVirus = NULL;

    // there is a node
    // its not the last
    while (curr_link->nextVirus != NULL){
        curr_link = curr_link->nextVirus;
    }
    // its the last, now add new one
    if (curr_link->nextVirus == NULL)
    {
        curr_link->nextVirus = new_link;
        return virus_list;
    }
    return virus_list;
}

/* Free the memory allocated by the list. */
void list_free(link *virus_list);

struct fun_desc
{
    char *name;
    link *(*fun)(link *);
};

int menu()
{
    FILE *fp;
    link *virus_list = malloc(sizeof(link));
    // Loop runs until invalid option declared
    while (1)
    {
        printf("Please choose a function:\n");

        printf("1) Load signatures\n");
        printf("2) Print signatures\n");

        printf("Option: ");
        int c = 0;

        scanf("%d", &c);
        // next line from stack overflow
        //discards unwanted chars from scanf
        while ((getchar()) != '\n')
            ;

        // If number received is in bounds - theres a function in this index
        if (c >= 1 && c <= (2))
        {
            printf("Within bounds\n");
            // Load signatures
            if (c == 1)
            {
                printf("Insert file name:\n");
                char userInput[100];
                fgets(userInput, 100, stdin);
                userInput[strlen(userInput) - 1] = '\0';
                fp = fopen(userInput, "r");
                if (fp == NULL)
                {
                    printf("No suitable file found\n");
                    exit(1);
                }
                fseek(fp, 0, SEEK_END);
                int size = ftell(fp);
                fseek(fp, 0, SEEK_SET);
                virus *vir0 = readVirus(fp);
                virus_list = list_append(virus_list, vir0);
                while (currIndex < size)
                {
                    list_append(virus_list, readVirus(fp));
                }
            }
            // Print signatures
            else
            {
                if (fp == NULL)
                {
                    exit(1);
                }
                list_print(virus_list, stdout);
            }

            printf("Done\n\n");
        }
        else
        {
            printf("Not within bounds\n");
            exit(0);
        }
    }
    if (fp != NULL)
        fclose(fp);
    return 0;
}

int main(int argc, char const *argv[])
{
    menu();
    return 0;
}
