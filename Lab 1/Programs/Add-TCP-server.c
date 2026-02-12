#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serveraddr, clientaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Socket creation error\n");
        return 0;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(sockfd, 1);

    printf("TCP Server waiting for client...\n");

    clilen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &clilen);

    printf("\nClient Connected (TCP)\n");
    printf("Client Socket Descriptor: %d\n", newsockfd);
    printf("Client IP Address: %s\n", inet_ntoa(clientaddr.sin_addr));
    printf("Client Port Number: %d\n", ntohs(clientaddr.sin_port));

    close(newsockfd);
    close(sockfd);
    return 0;
}
