#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

void deallocation(char **ar, int size) {
    int i;
    for(i = 0; i < size; i++)
        free(ar[i]);
}

int main(int argc, char *argv[], char *envp[]) {
    //char input[SIZE];
    char **args;
    char dir[256];
    size_t len = 0;
    char *input;

    while(1) {
        if (getcwd(dir, sizeof(dir)) == NULL) {
            perror("getcwd() error");
        }
        printf("%s$ ", dir);
        //fgets(input, SIZE, stdin);
        char *token;

        if(getline(&input, &len, stdin) == -1)
            printf("errore");

        token = strtok(input, " \n\t");
        args = (char **)malloc(sizeof(char *));
        args[0] = (char *)malloc(strlen(token) + 1);
        strcpy(args[0], token);

        int i = 1;
        while (token != NULL) {
            token = strtok(NULL, " \n\t");
            args = (char **) realloc (args, (i + 1) * sizeof(char *));
            if(token != NULL) {
                args[i] = (char *)malloc(strlen(token) + 1);
                strcpy(args[i], token);
                ++i;
            }
            
        }
        int j;
        
        if (!strcmp(args[0], "exit" )) exit(0);



        pid_t pid = fork();
        if (pid) {
            wait(NULL);
        } else {
            execvp(args[0], args);
            perror("Error calling exec()!\n");
            exit(1);
        }

        deallocation(args,i);

    }
    return 0;
}

