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
    socklen_t len;
    struct sockaddr_in serveraddr, clientaddr;
    char buff[50];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        printf("Socket creation error\n");
        return 0;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3399);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    printf("UDP Server waiting for datagram...\n");

    len = sizeof(clientaddr);
    recvfrom(sockfd, buff, sizeof(buff), 0,
             (struct sockaddr *)&clientaddr, &len);

    printf("\nClient Details (UDP)\n");
    printf("Client IP Address: %s\n", inet_ntoa(clientaddr.sin_addr));
    printf("Client Port Number: %d\n", ntohs(clientaddr.sin_port));

    close(sockfd);
    return 0;
}
