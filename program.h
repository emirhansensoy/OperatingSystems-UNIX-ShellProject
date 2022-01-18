#define MAX_WORD 10
#define MAX_CHAR 80

#define RED   "\x1B[31m"
#define BLU   "\x1B[34m"
#define RESET "\x1B[0m"

static char* currentDirectory;

int sau_cd(char **args);
int sau_showpid(char **args);
int sau_exit(char **args);
