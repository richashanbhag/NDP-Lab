#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define MAX 1024

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[MAX];
    char result[MAX * 10];
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        exit(1);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    len = sizeof(servaddr);

    while (1)
    {
        printf("\nEnter a string (or 'exit'): ");
        fgets(buffer, MAX, stdin);

        buffer[strcspn(buffer, "\n")] = '\0';

        sendto(sockfd, buffer, strlen(buffer), 0,
               (struct sockaddr *)&servaddr, len);

        if (strcmp(buffer, "exit") == 0)
            break;

        memset(result, 0, sizeof(result));

        recvfrom(sockfd, result, sizeof(result), 0,
                 (struct sockaddr *)&servaddr, &len);

        printf("\n[CLIENT OUTPUT] Permutations:\n%s", result);
    }

    close(sockfd);
    return 0;
}
