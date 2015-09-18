#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>

#define DIR_LENGTH 255  //Standard value for ext4 file system

void deallocation(char **array, int size) {
    int i;
    for(i = 0; i < size; i++)
        free(array[i]);
}

char **parseArguments(char *input, char *delim, int *size) {
    char *token;
    if((token = strtok(input, delim))  == NULL) {
        *size = -1;
        return NULL;
    }
    char **args = (char **)malloc(sizeof(char *));
    args[0] = (char *)malloc(strlen(token) + 1);
    strcpy(args[0], token);
    int args_size = 1;
    while (token != NULL) {
        token = strtok(NULL, delim);
        args = (char **) realloc (args, (args_size + 1) * sizeof(char *));
        if(token != NULL) {
            args[args_size] = (char *)malloc(strlen(token) + 1);
            strcpy(args[args_size], token);
            ++args_size;
        }

    }
    *size = args_size;
    return args;
}



int main(int argc, char *argv[], char *envp[]) {
    char cdCmd[] = "cd";
    char **args;
    int args_size;
    char **args1;
    int args1_size;
    char **args2;
    int args2_size;
    char dir[256];
    int fd[2];
    size_t size = 0;
    char *input;
    char *token;
    pid_t pid;

    while(1) {
        if (getcwd(dir, sizeof(dir)) == NULL) {
            printf("getcwd() error");
            exit(1);
        }
        printf("mysh1: %s$ ", dir);

        while(getline(&input, &size, stdin) == -1) {
            printf("Couldn't read the input\n");
            exit(1);
        }

        args = parseArguments(input, "|\n", &args_size);
        if(args_size == -1)
            continue;

        if(args_size == 1)
            if((token = strtok(input, " \t\n"))  == NULL)
                continue;

        //first command
        args1 = parseArguments(args[0], " \n\t", &args1_size);
        if(args1_size == -1) {
            printf("Error: no command before pipe!\n");
            exit(1);
        }

        if(args_size > 1) {

            //2nd command
            args2 = parseArguments(args[1], " \n\t", &args2_size);
            if(args2_size == -1) {
                printf("Error: no command after pipe!\n");
                exit(1);
            }

            if (pipe(fd) < 0) {
                perror("Error pipe");
                exit(1);
            }

            pid = fork();
            if (pid == 0) {
                dup2(fd[1], 1);
                if(execvp(args1[0], args1) == -1) {
                    //exit(1);
                    kill(getpid(),SIGTERM);
                }
            } else {
                close(fd[1]);
            }
            wait(NULL);

            pid = fork();
            if (pid == 0) {
                dup2(fd[0], 0);
                if(execvp(args2[0], args2) == -1) {
                    printf("Command not found!\n");
                    exit(1);
                }
            } else {
                close(fd[0]);
            }
            wait(NULL);
            deallocation(args1, args1_size);
            deallocation(args2, args2_size);
            deallocation(args, args_size);

        } else {
            if(!strcmp(args1[0], cdCmd)) {
                strcpy(dir, args1[1]); //Update current directory
                if(chdir(args1[1])) {
                    printf("%s is not a valid path.\n", args1[1]);
                }
            } else {
                pid_t pid = fork();
                if (pid == 0) {
                    execvp(args1[0], args1);
                    perror("Error calling exec()!\n");
                    exit(1);
                }
                wait(NULL);
                deallocation(args1, args1_size);
                deallocation(args, args_size);
            }
        }
    }
    return 0;
}
