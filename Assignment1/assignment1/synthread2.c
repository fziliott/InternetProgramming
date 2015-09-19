#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <pthread.h>
#include <string.h>

#define FALSE 0
#define TRUE 1

pthread_mutex_t mutex;
int ab_turn = TRUE;
pthread_cond_t cond_p;
pthread_cond_t cond_c;

/*code for first pthread*/
void *printFirst(void *s) {
    int i;
    for (i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex); //ask to access the critical section

        /*if it is not my turn wait the condition*/
        while (ab_turn == FALSE)
            pthread_cond_wait(&cond_p, &mutex);

        /*set the turn for the other pthread*/
        ab_turn = FALSE;
        printf("ab");
        pthread_cond_signal(&cond_c);   //notify the other pthread
        pthread_mutex_unlock(&mutex);   //exit the critical section
    }
}

/*code for second pthread*/
void *printCd(void *s) {
    int i;
    for (i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex); //ask to access the critical section
        /*if it is not my turn wait the condition*/
        while (ab_turn == TRUE)
            pthread_cond_wait(&cond_c, &mutex);
        /*set the turn for the other pthread*/
        ab_turn = TRUE;
        printf("cd\n");
        pthread_cond_signal(&cond_p);    //notify the other pthread
        pthread_mutex_unlock(&mutex);   //exit the critical section
    }

}

int main() {
    void (*func_ptr[2]) = {printAb, printCd}; //array of function pointers
    pthread_t tid[2];   //array of pthread pointers
    int i, err;

    /*initialize the mutex*/
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("\n mutex init failed\n");
        return 1;
    }

    /*initialize the conditions*/
    pthread_cond_init(&cond_p, NULL);
    pthread_cond_init(&cond_c, NULL);

    /*create 2 pthreads with relative functions*/
    i = 0;
    while(i < 2) {
        err = pthread_create(&(tid[i]), NULL, func_ptr[i], NULL);
        if (err != 0)
            printf("Can't create thread :[%s]", strerror(err));
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_mutex_destroy(&mutex);

    return 0;
}

