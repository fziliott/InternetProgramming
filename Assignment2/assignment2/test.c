#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>


int main(int argc, char const *argv[]) {
    int num = 0;

    while(num < 50) {
        if(fork() == 0) {
        	usleep(100);
            int socketfd;
            struct sockaddr_in remote;

            if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                perror("socket");
                exit(1);
            }

            remote.sin_family = AF_INET;
            remote.sin_port = htons(4444);
            remote.sin_addr.s_addr = inet_addr("127.0.0.1");
            if (connect(socketfd, (struct sockaddr *)&remote, sizeof(struct sockaddr_in)) == -1) {
                perror("connect");
                exit(1);
            }
            int buf;
            if((recv(socketfd, &buf, sizeof(int), 0)) == -1) {
                perror("read");
                exit(1);
            }
            printf("%d: I received: %d\n", getpid(), buf);
            close(socketfd);
            exit(1);
        }
        ++num;
    }
    
    /* code */
    return 0;
}