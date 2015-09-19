#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/sem.h>

void display(char *str) {
    char *tmp;
    for (tmp = str; *tmp; tmp++) {
        write(1, tmp, 1);
        usleep(100);
    }
}

int main() {
    struct sembuf ab_up =   {0, 1, 0};  //struct for the UP() operation on semaphore 0
    struct sembuf ab_down = {0, -1, 0}; //struct for the DOWN() operation on semaphore 0
    struct sembuf cd_up =   {1, 1, 0};  //struct for the UP() operation on semaphore 1
    struct sembuf cd_down = {1, -1, 0}; //struct for the DOWN() operation on semaphore 1
    int i;
    int sem;

    sem = semget(IPC_PRIVATE, 2, 0600); /* create semaphore */
    semop(sem, &ab_up, 1);              //at the begin the semaphore must be set to 1 (open)

    if (fork()) {
        for (i = 0; i < 10; i++) {
            semop(sem, &ab_down, 1);    //ask to access the critical section
            display("ab");
            semop(sem, &cd_up, 1);      //unlock the semaphore for the other process
        }
        wait(NULL);
    } else {
        for (i = 0; i < 10; i++) {
            semop(sem, &cd_down, 1);    //ask to access the critical section
            display("cd\n");
            semop(sem, &ab_up, 1);      //unlock the semaphore for the other process
        }
    }
    return 0;
}
