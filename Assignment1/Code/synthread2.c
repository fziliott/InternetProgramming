#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <pthread.h>

#define FALSE 0
#define TRUE 0

pthread_mutex_t mutex;
int ab_written = FALSE;
int cd_written = TRUE;
pthread_cond_t cond_p;
pthread_cond_t cond_c;

void *prod(void *s) {
    int i;
    for (i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);
        while (cd_written == FALSE)
            pthread_cond_wait(&cond_p, &mutex);
        cd_written = FALSE;
        ab_written = TRUE;

        pthread_cond_signal(&cond_c);
        pthread_mutex_unlock(&mutex);
    }
}

void *cons(void *s) {
    int i;
    for (i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);
        while (ab_written == FALSE)
            pthread_cond_wait(&cond_c, &mutex);
        ab_written = FALSE;
        cd_written = TRUE;
        printf("%s\n", s);
        pthread_cond_signal(&cond_p);
        pthread_mutex_unlock(&mutex);
    }

}

int main() {
    char *s[] = {"ab", "cd"};
    pthread_t tid[2];
    int i, err;
    //void* (*apfunz[2])(void*)={cons,prod};


    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("\n mutex init failed\n");
        return 1;
    }
    pthread_cond_init(&cond_p, NULL);
    pthread_cond_init(&cond_c, NULL);
    /*while(i < 2) {
        printf("B");
        err = pthread_create(&(tid[i]), NULL, (apfunz[i]), s[i]);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        i++;
    }*/

    err = pthread_create(&(tid[0]), NULL, &cons, s[1]);
    if (err != 0) {
        printf("\ncan't create thread :[%s]", strerror(err));
        return 1;
    }

    err = pthread_create(&(tid[1]), NULL, &prod, s[0]);
    if (err != 0) {
        printf("\ncan't create thread :[%s]", strerror(err));
        return 1;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_mutex_destroy(&mutex);

    return 0;

}

