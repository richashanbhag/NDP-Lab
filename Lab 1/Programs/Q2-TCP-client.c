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
    int sockfd, retval;
    int sentbytes, recedbytes;
    struct sockaddr_in serveraddr;
    char buff[MAXSIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Socket Creation Error\n");
        return 0;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    retval = connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1)
    {
        printf("Connection Error\n");
        return 0;
    }

    while (1)
    {
        printf("\nEnter a string: ");
        scanf("%s", buff);

        send(sockfd, buff, strlen(buff) + 1, 0);

        if (strcmp(buff, "Halt") == 0)
            break;

        memset(buff, 0, sizeof(buff));
        recedbytes = recv(sockfd, buff, sizeof(buff), 0);

        if (recedbytes > 0)
            printf("Server Response: %s\n", buff);
    }

    close(sockfd);
    return 0;
}
