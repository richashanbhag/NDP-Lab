#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define MAXSIZE 100

int main()
{
    int sockfd;
    struct sockaddr_in serveraddr;
    socklen_t len;
    char buff[MAXSIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        printf("Socket Creation Error\n");
        return 0;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3399);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    len = sizeof(serveraddr);

    while (1)
    {
        printf("\nEnter a string: ");
        scanf("%s", buff);

        sendto(sockfd, buff, strlen(buff) + 1, 0,
               (struct sockaddr *)&serveraddr, len);

        if (strcmp(buff, "Halt") == 0)
            break;

        memset(buff, 0, sizeof(buff));

        recvfrom(sockfd, buff, sizeof(buff), 0,
                 (struct sockaddr *)&serveraddr, &len);

        printf("Server Response: %s\n", buff);
    }

    close(sockfd);
    return 0;
}
