#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

const int SIZE = 42;

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
    char input[SIZE];
    char *args[2];
    args[1]=NULL;
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

