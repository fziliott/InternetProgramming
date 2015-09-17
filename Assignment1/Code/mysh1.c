#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#define DIR_LENGTH 255  //Standard value for ext4 file system

int main(int argc, char *argv[], char *envp[])
{
    char *input=NULL;
    size_t size = 0;
    char *args[2];
    args[1] = NULL;
    char dir[DIR_LENGTH];
    char *token;

    while(1)
    {
        if (getcwd(dir, sizeof(dir)) == NULL)
        {
            perror("getcwd() error");
        }
        printf("mysh1: %s$ ", dir);
        while(getline(&input, &size, stdin) == -1)
        {
            perror("Couldn't read the input\n");
            exit(1);
        }

        token = strtok(input, " \n\t");
        args[0] = token;

        if (!strcmp(args[0], "exit" )) exit(0);

        pid_t pid = fork();
        if (pid)
        {
            wait(NULL);
        }
        else
        {
            execvp(args[0], args);
            perror("Error calling exec()!\n");
            exit(1);
        }

    }
    return 0;
}

