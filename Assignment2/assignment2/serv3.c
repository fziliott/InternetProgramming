#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define BACKLOG 5
#define PORT 4444
#define NB_PROC 10

int id;
int *counter;
int mutex;
int socketfd;

struct sembuf acceptUp =   {0, 1, 0};
struct sembuf counterUp =   {1, 1, 0};
struct sembuf acceptDown = {0, -1, 0};
struct sembuf counterDown = {1, -1, 0};

void sig_chld(int a);
void handler() {
    shmdt((void*) counter);
    shmctl(id, IPC_RMID, 0);
    close(socketfd);
    //printf("%d\n", socketfd);
    //printf("closing\n" );
    semctl(mutex, 0, IPC_RMID);
    exit(0);
}

ssize_t writen(int fd, const void *vptr, size_t n);

int openSocket(struct sockaddr_in *server_addr);

void recv_requests(int fd);

void treat_request(int socketfd);



int main(void) {
    int num;
    struct sockaddr_in server_addr;

    mutex = semget(IPC_PRIVATE, 2, 0600);     //semaphore
    semop(mutex, &acceptUp, 1);     //at the begin the semaphore must be set to 1 (open)
    semop(mutex, &counterUp, 1);     //at the begin the semaphore must be set to 1 (open)

    id = shmget(IPC_PRIVATE, sizeof(int), 0600 | IPC_CREAT);
    if (id < 0) {
        perror("Error shmget");
        exit(1);
    }
    counter = (int *) shmat(id, 0, 0);
    *counter = 0;

    socketfd = openSocket(&server_addr);
    signal(SIGINT, handler);

    for (num = 0; num < NB_PROC; num++){
        if (fork() == 0) {
            signal(SIGINT, handler);
            recv_requests(socketfd);
        }
    }
    int * status;
    printf("%d\n", getpid());
//waitpid(-1, &status, 0);
    int cont=NB_PROC;
    while(cont>0){
        wait(NULL);
        printf("%d\n", NB_PROC-cont);
        cont--;
    }
}


void treat_request(int sfd) {
    semop(mutex, &counterDown, 0);
    (*counter)++;
    uint32_t msg = htonl(*counter);
    semop(mutex, &counterUp, 0);

    if (writen(sfd, (void *)&msg, sizeof(int)) == -1) {
        perror("send");
    }
}

void recv_requests(int sfd) { /* An iterative server */

    struct sockaddr_in client_addr;
    int addrlen = sizeof(struct sockaddr_in);

    while (1) {
        semop(mutex, &acceptDown, 1);
        int newsfd = accept(sfd, (struct sockaddr *) &client_addr, &addrlen);
        semop(mutex, &acceptUp, 1);

        if(newsfd < 0)
            continue;
        treat_request(newsfd);
        close(newsfd);
    }
}

int openSocket(struct sockaddr_in *server_addr) {
    int fd;
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    int enable = 1;
    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("Error setting socket options");
    }
     if(setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0) {
        perror("Error setting socket options");
    }

    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(PORT);
    server_addr->sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(fd, (struct sockaddr *) server_addr,
             sizeof(struct sockaddr_in)) == -1) {
        perror("bind");
    }

    if (listen(fd, BACKLOG) == -1)
        perror("listen");

    return fd;
}



ssize_t writen(int fd, const void *vptr, size_t n) {
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;
    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if  ((nwritten = write(fd, ptr, nleft)) <= 0 ) {
            if (errno == EINTR)
                nwritten = 0; /* and call write() again */
            else
                return -1; /* error */
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}


void sig_chld(int a) {
    while (waitpid(0, NULL, WNOHANG) > 0) {}
    signal(SIGCHLD, sig_chld);
}