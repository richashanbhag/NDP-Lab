#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main()
{
    int sockfd;
    struct sockaddr_in serveraddr;
    char buff[] = "Hello";

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3399);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    sendto(sockfd, buff, strlen(buff) + 1, 0,
           (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    printf("UDP Client sent data\n");

    close(sockfd);
    return 0;
}
