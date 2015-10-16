#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>


/*this function takes an array of pointers to memory locations that were previously allocated
with malloc and deallocates them by invoking free for each pointer.*/
void deallocation(char **array, int size) {
    int i;
    for(i = 0; i < size; i++)
        free(array[i]);
}

/*takes the string in input and a character array of delimiters and returns an array
of pointers to tokens that are obtained from the initial input by separating the tokens with
the delim's characters. The number of tokens is saved in the variable size
and the function return the array of pointers to the tokens.*/
char **parseArguments(char *input, char *delim, int *size) {
    char *token;
    int args_size;
    char **args;

    if((token = strtok(input, delim))  == NULL) {
        *size = -1;
        return NULL;
    }
    args = (char **)malloc(sizeof(char *));
    args[0] = (char *)malloc(strlen(token) + 1);
    strcpy(args[0], token);
    args_size = 1;
    while (token != NULL) {
        token = strtok(NULL, delim);
        args = (char **) realloc (args, (args_size + 1) * sizeof(char *));
        if(token != NULL) {
            args[args_size] = (char *)malloc(strlen(token) + 1);
            strcpy(args[args_size], token);
            ++args_size;
        } else {
            args[args_size] = NULL;
        }

    }
    *size = args_size;
    return args;
}



int main(int argc, char *argv[], char *envp[]) {
    char cdCmd[] = "cd";
    char **args;
    int args_size;      //number of elements in args
    char **args1;       //array of string, it will contains the first command, the relative attributes and a NULL in the end
    int args1_size;     //number of elements in args1
    char **args2;       //array of string, it will contains the second command, the relative attributes and a NULL in the end
    int args2_size;     //number of elements in args2
    int fd[2];          //pipe
    char *input;        //string inserted by the user
    size_t size = 0;    //size of the string inserted by the user
    char *token;
    int pipe_found = 0; //say if a pipe symbol is found on the string inserted by the user or not
    char *pch;

    while(1) {
        printf("$");

        /*gets a string from the user*/
        while(getline(&input, &size, stdin) == -1) {
            printf("Couldn't read the input\n");
            exit(1);
        }

        /*searchs a pipe in the string inserted by the user*/
        pch = strchr(input, '|');
        if (pch != NULL)
            pipe_found = 1;

        /*splits the string inserted by the user in tokens (commands), if the user inserted
                anything continue ask for other commands*/
        args = parseArguments(input, "|\n", &args_size);
        if(args_size == -1)
            continue;

        /*checks if there is a pipe symbol but only one command*/
        if (pipe_found && args_size == 1) {
            printf("Error: no command before or after pipe!\n");
            pipe_found = 0;
            deallocation(args, args_size);
            continue;
        }
        pipe_found = 0;

        /*if the user inserted anything (only spaces or tabs) continue ask for other commands*/
        if(args_size == 1)
            if((token = strtok(input, " \t\n"))  == NULL)
                continue;

        /*splits the first command in tokens (command + arguments), if the user inserted
                anything print the error and continue ask for other commands*/
        args1 = parseArguments(args[0], " \n\t", &args1_size);
        if(args1_size == -1) {
            printf("Error: no command before pipe!\n");
            deallocation(args, args_size);
            continue;
        }

        /*check if there are 2 commands*/
        if(args_size > 1) {

            /*splits the second command in tokens (command + arguments), if the user inserted
                anything print the error and continue ask for other commands*/
            args2 = parseArguments(args[1], " \n\t", &args2_size);
            if(args2_size == -1) {
                printf("Error: no command after pipe!\n");
                deallocation(args1, args1_size);
                deallocation(args, args_size);
                continue;
            }

            /*pipe creation*/
            if (pipe(fd) < 0) {
                perror("Error pipe");
                exit(1);
            }

            /*creates another process, redirect its stdout to the pipe and try to exec the first retrieved
            command with relative attributes*/
            if (fork() == 0) {
                if (!strcmp(args1[0], "exit" )) exit(0);

                if(!strcmp(args1[0], cdCmd)) {

                    /*if the retrieved command is 'cd' than we update the current directory*/
                    if(args1[1] == NULL) {
                        if(chdir("/")) {
                            printf("%s is not a valid path.\n", args1[1]);
                        }
                    } else {
                        if(chdir(args1[1])) {
                            printf("%s is not a valid path.\n", args1[1]);
                        }
                    }
                } else {
                    dup2(fd[1], 1);
                    if(execvp(args1[0], args1) == -1) {
                        kill(getpid(), SIGTERM);
                    }
                }
            } else {
                close(fd[1]);
            }
            wait(NULL);

            /*creates another process, redirect its stdin to the pipe and try to exec the second retrieved
            command with relative attributes*/
            if (fork() == 0) {
                if (!strcmp(args2[0], "exit" )) exit(0);

                /*if the retrieved command is 'cd' than we update the current directory*/
                if(!strcmp(args2[0], cdCmd)) {
                    if(args2[1] == NULL) {
                        if(chdir("/")) {
                            printf("%s is not a valid path.\n", args2[1]);
                        }
                    } else {
                        if(chdir(args2[1])) {
                            printf("%s is not a valid path.\n", args2[1]);
                        }
                    }
                } else {
                    dup2(fd[0], 0);
                    if(execvp(args2[0], args2) == -1) {
                        printf("Command not found!\n");
                        exit(1);
                    }
                }
            } else {
                close(fd[0]);
            }
            wait(NULL);
            deallocation(args1, args1_size);
            deallocation(args2, args2_size);
            deallocation(args, args_size);

        } else {    /*there is only one command*/
            if (!strcmp(args1[0], "exit" )) exit(0);
            /*if the retrieved command is 'cd' than we update the current directory*/
            if(!strcmp(args1[0], cdCmd)) {
                if(args1[1] == NULL) {
                    if(chdir("/")) {
                        printf("%s is not a valid path.\n", args1[1]);
                    }
                } else {
                    if(chdir(args1[1])) {
                        printf("%s is not a valid path.\n", args1[1]);
                    }
                }
            } else {
                /*creates another process and try to exec the retrieved command with
                relative attributes*/
                if (fork() == 0) {
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
