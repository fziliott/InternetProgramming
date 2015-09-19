#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#define DIR_LENGTH 255  //Standard value for ext4 file system

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
    char dir[DIR_LENGTH];   //current directory path
    char *input;            //string inserted by the user
    size_t size = 0;        //size of the string inserted by the user
    char **args;            //array of string, it will contains the command, the relative attributes and a NULL in the end
    int args_size;          //number of elements in args
    char *token;

    while(1) {
        /*gets the current directory path and print it before the $ symbol*/
        if (getcwd(dir, sizeof(dir)) == NULL) {
            printf("getcwd() error");
            exit(1);
        }
        printf("mysh2: %s$ ", dir);

        /*gets a string from the user*/

        while(getline(&input, &size, stdin) == -1) {
            printf("Couldn't read the input\n");
            exit(1);
        }

        /*splits the string inserted by the user in tokens (command + attributes), if the user inserted
        anything continue ask for other commands*/
        args = parseArguments(input, " \t\n", &args_size);
        if(args_size == -1) {
            continue;
        }

        /*creates another process and try to exec the retrieved command with 
        relative attributes*/
        if (!strcmp(args[0], "exit" )) exit(0);

        if (fork()) {
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

