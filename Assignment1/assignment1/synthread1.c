#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <pthread.h>
#include <string.h>

pthread_mutex_t mutex;

void display(char *str) {
    char *tmp;
    for (tmp = str; *tmp; tmp++) {
        write(1, tmp, 1);
        usleep(100);
    }
}

void *print(void *s) {
    int i;
    for (i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);     //ask to access the critical section
        display((char *) s);
        pthread_mutex_unlock(&mutex);   //exit the critical section
    }
}

int main() {
    char *s[] = {"Hello World\n", "Bonjour Monde\n"};
    pthread_t tid[2];       //array of pthread pointers
    int i, err;

    /*initialize the mutex*/
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("\n mutex init failed\n");
        return 1;
    }

    /*create 2 pthreads with relative strings to print*/
    i = 0;
    while(i < 2) {
        err = pthread_create(&(tid[i]), NULL, print, s[i]);
        if (err != 0)
            printf("Can't create thread :[%s]", strerror(err));
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_mutex_destroy(&mutex);

    return 0;

}

