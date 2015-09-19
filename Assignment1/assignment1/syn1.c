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
    int i;
    int sem = semget(IPC_PRIVATE, 1, 0600);
    struct sembuf up =   {0, 1, 0};
    struct sembuf down = {0, -1, 0};
    semop(sem, &up, 1);
    if (fork()) {
        for (i = 0; i < 10; i++) {
            semop(sem, &down, 1);
            display("Hello world\n");
            semop(sem, &up, 1);
        }
        wait(NULL);
    } else {
        for (i = 0; i < 10; i++) {
            semop(sem, &down, 1);
            display("Bonjour monde\n");
            semop(sem, &up, 1);
        }
    }
    return 0;
}