#include <stdio.h>
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
    int i;                                      //index
    int sem = semget(IPC_PRIVATE, 1, 0600);     //semaphore
    struct sembuf up =   {0, 1, 0};             //struct for the UP() operation on semaphore 
    struct sembuf down = {0, -1, 0};            //struct for the DOWN() operation on semaphore

    semop(sem, &up, 1);     //at the begin the semaphore must be set to 1 (open)

    if (fork()) {
        for (i = 0; i < 10; i++) {
            semop(sem, &down, 1);       //ask to access the critical section
            display("Hello world\n");
            semop(sem, &up, 1);         //exit the critical section
        }
        wait(NULL);
    } else {
        for (i = 0; i < 10; i++) {
            semop(sem, &down, 1);       //ask to access the critical section
            display("Bonjour monde\n");
            semop(sem, &up, 1);         //exit the critical section
        }
    }
    semctl(sem, 0, IPC_RMID);
    return 0;
}