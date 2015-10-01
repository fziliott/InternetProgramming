#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MSG_SIZE 4
#define PORT 4444


int main(int argc, char **argv) {
    struct hostent *host;
    if(argc < 2) {
        printf("Couldn't get server name!\n");
        exit(1);
    }
    int socketfd;
    struct sockaddr_in remote;

    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    host = gethostbyname(argv[1]);
    if (host == NULL) {
        fprintf(stderr, "Hostname \"%s\" could not be resolved\n", argv[1]);
        exit(-2);
    }
    char *addr[5];
    int i = 0;
    struct in_addr **q = (struct in_addr **)host->h_addr_list;
    while (*q != NULL) {
        addr[i] = malloc(16 * sizeof(char));
        strcpy(addr[i], inet_ntoa(**q));
        q++;
        ++i;
    }

    remote.sin_family = AF_INET;
    remote.sin_port = htons(PORT);
    remote.sin_addr.s_addr = inet_addr(addr[0]);
    if (connect(socketfd, (struct sockaddr *)&remote, sizeof(struct sockaddr_in)) == -1) {
        perror("connect");
        exit(1);
    }
    int buf;
    if((recv(socketfd, &buf, sizeof(int), 0)) == -1) {
        perror("read");
        exit(1);
    }
    int msg = ntohl(buf);
    printf("I received: %d\n", msg);

    close(socketfd);

    return 0;
}
