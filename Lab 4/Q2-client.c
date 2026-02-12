#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;
    char domain[MAX], response[MAX];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    while (1)
    {
        printf("\nEnter domain name (or 'exit'): ");
        scanf("%s", domain);

        send(sockfd, domain, strlen(domain) + 1, 0);

        if (strcmp(domain, "exit") == 0)
            break;

        recv(sockfd, response, MAX, 0);
        printf("[CLIENT] DNS Response: %s\n", response);
    }

    close(sockfd);
    return 0;
}
