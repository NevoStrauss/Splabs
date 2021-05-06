#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int currIndex;
char endian;

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
    unsigned short sigSize;

    sigSize = calcLitlleEndian(sizeAndName[0], sizeAndName[1]);

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
    while (curr_link->nextVirus != NULL)
    {
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

void detect_virus(char *buffer, unsigned int size, link *virus_list, FILE *output)
{
    link *currLink = virus_list;
    virus *currVirus = virus_list->vir;

    // Iterating each byte
    for (size_t i = 0; i < size; i++)
    {
        // Iterating each signature
        while ((currLink != NULL) && (currVirus != NULL))
        {
            if (memcmp(buffer + i, currVirus->sig, currVirus->sigSize) == 0)
            {
                printf("**A virus has been detected!**\n");
                printf("Starts in byte: %d\n", i);
                printf("Virus name: %s\n", currVirus->virusName);
                printf("Virus signature's size: %d\n", currVirus->sigSize);
            }

            currLink = currLink->nextVirus;
            if (currLink != NULL)
            {
                currVirus = currLink->vir;
            }
        }
        currLink = virus_list;
        currVirus = virus_list->vir;
    }
}

/* Free the memory allocated by the list. */
void list_free(link *virus_list);

struct fun_desc
{
    char *name;
    link *(*fun)(link *);
};

int menu(int argc, const char **argv)
{

    FILE *fp;
    FILE *fp3;
    link *virus_list = malloc(sizeof(link));
    short fpOpened = 0;
    short fp3Opened = 0;
    unsigned int fpsize;
    // Loop runs until invalid option declared
    while (1)
    {
        printf("Please choose a function:\n");

        printf("1) Load signatures\n");
        printf("2) Print signatures\n");
        printf("3) Detect viruses\n");
        printf("Option: ");
        int c = 0;

        scanf("%d", &c);
        // next line from stack overflow
        //discards unwanted chars from scanf
        while ((getchar()) != '\n')
            ;

        // If number received is in bounds - theres a function in this index
        if (c >= 1 && c <= 3)
        {
            printf("Within bounds\n");
            // Load signatures
            if (c == 1)
            {
                printf("Insert file name:\n");
                char userInput[100];
                fgets(userInput, 100, stdin);
                userInput[strlen(userInput) - 1] = '\0';
                fp = fopen(userInput, "r+");
                if (fp == NULL)
                {
                    printf("No suitable file found\n");
                    exit(1);
                }
                else
                {
                    fpOpened = 1;
                }
                fseek(fp, 0, SEEK_END);
                fpsize = ftell(fp);
                fseek(fp, 0, SEEK_SET);
                virus *vir0 = readVirus(fp);
                virus_list = list_append(virus_list, vir0);
                while (currIndex < fpsize)
                {
                    list_append(virus_list, readVirus(fp));
                }
            }
            // Print signatures
            else if (c == 2)
            {
                if (fp == NULL)
                {
                    exit(1);
                }
                list_print(virus_list, stdout);
            }
            // Detect Viruses
            else
            {

                if (!fpOpened)
                {
                    printf("fp is null\n");
                    exit(1);
                }

                if (argc < 1)
                {
                    printf("No suspected file name provide in command line.");
                    exit(1);
                }

                fseek(fp, 0, SEEK_SET);

                // FILE *fp3 = fopen(argv[1], "r");
                int namelen = strlen(argv[1]);
                char filename[namelen + 1];
                memcpy(filename, argv[1], namelen);
                filename[namelen] = '\0';
                fp3 = fopen(filename, "r+");
                if (fp3 == NULL)
                {
                    printf("fp3 is null\n");
                }
                else
                {
                    fp3Opened = 1;
                }

                char buffer[10000];
                // char buffer2[850];
                fread(buffer, 1, fpsize, fp3);
                // fread(buffer2, 1, 850, fp3);
                fseek(fp, 0, SEEK_SET);

                fseek(fp3, 0, SEEK_END);
                unsigned int detVirSize = ftell(fp3);
                fseek(fp3, 0, SEEK_SET);

                if (fpsize < detVirSize)
                {
                    detVirSize = fpsize;
                }
                detect_virus(buffer, detVirSize, virus_list, stdout);
            }

            printf("Done\n\n");
        }
        else
        {
            printf("Not within bounds\n");
            if (fpOpened)
                fclose(fp);
            if (fp3Opened)
                fclose(fp3);
            exit(0);
        }
    }

    return 0;
}

/* Free the memory allocated by the list. */
void list_free(link *virus_list)
{
    link *tmp = virus_list;
    while (virus_list != NULL)
    {
        tmp = virus_list;
        virus_list = virus_list->nextVirus;
        free(tmp->nextVirus);
        free(tmp->vir);
        free(tmp);
    }
}

int main(int argc, char const *argv[])
{
    menu(argc, argv);
    return 0;
}
