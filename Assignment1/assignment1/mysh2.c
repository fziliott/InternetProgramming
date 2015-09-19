#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#define DIR_LENGTH 255  //Standard value for ext4 file system

void deallocation(char **array, int size) {
    int i;
    for(i = 0; i < size; i++)
        free(array[i]);
}

int main(int argc, char *argv[], char *envp[]) {
    char dir[DIR_LENGTH];
    char *input;
    char **args;
    int args_size;
    size_t size = 0;
    char *token;
    pid_t pid;

    while(1) {
        if (getcwd(dir, sizeof(dir)) == NULL) {
            printf("getcwd() error");
            exit(1);
        }
        printf("mysh2: %s$ ", dir);

        while(getline(&input, &size, stdin) == -1) {
            printf("Couldn't read the input\n");
            exit(1);
        }

        if((token = strtok(input, " \t\n"))  == NULL)
            continue;

        args = (char **)malloc(sizeof(char *));
        args[0] = (char *)malloc(strlen(token) + 1);
        strcpy(args[0], token);

        args_size = 1;
        while (token != NULL) {
            token = strtok(NULL, " \n\t");
            args = (char **) realloc (args, (args_size + 1) * sizeof(char *));
            if(token != NULL) {
                args[args_size] = (char *)malloc(strlen(token) + 1);
                strcpy(args[args_size], token);
                ++args_size;
            }
        }

        if (!strcmp(args[0], "exit" )) exit(0);

        pid = fork();
        if (pid) {
            wait(NULL);
        } else {
            if(execvp(args[0], args) == -1)
                printf("Command not found!\n");
            exit(1);
        }

        deallocation(args, args_size);

    }
    return 0;
}

