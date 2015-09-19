#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <pthread.h>
#include <string.h>

pthread_mutex_t mutex;

void *print(void *s) {
    int i;
    for (i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);
        printf("%s\n", (char*)s);
        pthread_mutex_unlock(&mutex);
    }
}

int main() {
    char *s[] = {"Hello World", "Bonjour Monde"};
    pthread_t tid[2];
    int i, err;


    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("\n mutex init failed\n");
        return 1;
    }
    
    i=0;
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

