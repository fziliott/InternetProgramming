#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

const int SIZE = 42;
const int NARGS = 10;
const int NCOM = 2;

void printString(char *s, int size) {
    while(size > 0) {
        printf("Print string: %s\n", s);
        ++s;
        --size;
    }
}

int main(int argc, char *argv[], char *envp[]) {
    char cdCmd[] = "cd";
    char input[SIZE];
    char *args[NARGS];
    char *args1[NARGS];
    char *args2[NARGS];
    char dir[256];
    int p[2];

    while(1) {
        if (getcwd(dir, sizeof(dir)) == NULL) {
            perror("getcwd() error");
        }
        printf("%s$ ", dir);
        fgets(input, SIZE, stdin);
        char *token;

        token = strtok(input, "|");
        args[0] = token;

        int i = 1;
        int j;
        int n = 1;
        while (token != NULL && n <= NCOM) {
            token = strtok(NULL, " \n\t");
            args[n] = token;
            ++n;
        }

        //1st command
        token = strtok(args[0], " \n\t");
        args1[0] = token;

        while (token != NULL && i <= NARGS) {
            token = strtok(NULL, " \n\t");
            args1[i] = token;
            ++i;
        }

        if(n > 2) {
            //2nd command
            token = strtok(args[1], " \n\t");
            args2[0] = token;

            i = 1;
            while (token != NULL && i <= NARGS) {
                token = strtok(NULL, " \n\t");
                args2[i] = token;
                ++i;
            }

            if (pipe(p) < 0) {
                perror("Error pipe");
                exit(1);
            }

            pid_t pid = fork();
            if (pid == 0) {
                dup2(p[1], 1);
                execvp(args1[0], args1);
                perror("Error calling exec()!\n");
                exit(1);
            } else {
                /* Close the input side of the pipe, to prevent it staying open. */
                close(p[1]);
            }

            pid = fork();
            if (pid == 0) {
                dup2(p[0], 0);
                execvp(args2[0], args2);
                perror("Error calling exec()!\n");
                exit(1);
            }
            waitpid(pid);
        } else {
            if(!strcmp(args1[0], cdCmd)) {
                strcpy(dir, args1[1]); //Update current directory
                if(chdir(args1[1])) {
                    printf("%s is not a valid path.\n", args1[1]);
                }
            } else {
                pid_t pid = fork();
                if (pid) {
                    wait(NULL);
                } else {
                    execvp(args1[0], args1);
                    perror("Error calling exec()!\n");
                    exit(1);
                }
            }
        }
    }
    return 0;
}

