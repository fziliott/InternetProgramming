#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#define DIR_LENGTH 255  //Standard value for ext4 file system

int main(int argc, char *argv[], char *envp[])
{
    char dir[DIR_LENGTH];	//current directory path  
    char *input = NULL;		//string inserted by the user
    size_t size = 0;		//size of the string inserted by the user
    char *args[2];		//array of string, the first is the command and the second NULL

    while(1)
    {
	/*get the current directory path and print it before the $ symbol*/
        if (getcwd(dir, sizeof(dir)) == NULL)
        {
            printf("getcwd() error");
            exit(1);
        }
        printf("mysh1: %s$ ", dir);
	
	/*get a string from the user*/
        while(getline(&input, &size, stdin) == -1)
        {
            printf("Couldn't read the input\n");
            exit(1);
        }

	/*get the first word of the string inserted by the user, if the user inserted anything continue ask for other commands*/
        if((args[0] = strtok(input, " \t\n"))  == NULL)
            continue;
        args[1] = NULL;

        if (!strcmp(args[0], "exit" )) exit(0);

	/*create another process and try to exec the retrieved command*/
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

