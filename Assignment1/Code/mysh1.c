#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

const int SIZE = 42;
const int NARGS = 10;

void printString(char *s, int size)
{
    while(size > 0)
    {
        printf("Print string: %s\n", s);
        ++s;
        --size;
    }
}

int main(int argc, char *argv[], char *envp[])
{
    char cdCmd[] = "cd";
    char input[SIZE];
    char *args[NARGS];
    char *ptr;
    char dir[256];

    while(1)
    {
        if (getcwd(dir, sizeof(dir)) == NULL)
        {
            perror("getcwd() error");
        }
        printf("%s$ ", dir);
        fgets(input, SIZE, stdin);
        char *token;

        token = strtok(input, " \n\t");
        args[0] = token;

        int i = 1;
        while (token != NULL && i <= NARGS)
        {
            token = strtok(NULL, " \n\t");
            args[i] = token;
            ++i;
        }

        if (!strcmp(args[0], "exit" )) exit(0);
        if(!strcmp(args[0], cdCmd))
        {
            strcpy(dir, args[1]); //Update current directory
            if(chdir(args[1]))
            {
                printf("%s is not a valid path.\n", args[1]);
            }
        }
        else
        {
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
    }
    return 0;
}

