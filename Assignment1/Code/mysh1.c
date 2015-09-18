#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#define DIR_LENGTH 255  //Standard value for ext4 file system

int main(int argc, char *argv[], char *envp[])
{
    char dir[DIR_LENGTH];
    char *input = NULL;
    size_t size = 0;
    char *args[2];

    while(1)
    {
        if (getcwd(dir, sizeof(dir)) == NULL)
        {
            printf("getcwd() error");
            exit(1);
        }
        printf("mysh1: %s$ ", dir);
        while(getline(&input, &size, stdin) == -1)
        {
            printf("Couldn't read the input\n");
            exit(1);
        }

        if((args[0] = strtok(input, " \t\n"))  == NULL)
            continue;
        args[1] = NULL;

        if (!strcmp(args[0], "exit" )) exit(0);

        pid_t pid = fork();
        if (pid)
        {
            wait(NULL);
        }
        else
        {
            if(execvp(args[0], args) == -1)
                printf("Command not found!\n");
            exit(1);
        }
    }
    return 0;
}

