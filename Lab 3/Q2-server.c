#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define MAX 1024

void swap(char *x, char *y)
{
    char temp = *x;
    *x = *y;
    *y = temp;
}

void permute(char *str, int l, int r, char *result)
{
    if (l == r)
    {
        strcat(result, str);
        strcat(result, "\n");
        return;
    }
    for (int i = l; i <= r; i++)
    {
        swap(&str[l], &str[i]);
        permute(str, l + 1, r, result);
        swap(&str[l], &str[i]);
    }
}

int main()
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    char buffer[MAX];
    char result[MAX * 10];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        exit(1);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    printf("UDP Server waiting for strings...\n");

    len = sizeof(cliaddr);

    while (1)
    {
        memset(buffer, 0, MAX);
        memset(result, 0, sizeof(result));

        recvfrom(sockfd, buffer, MAX, 0,
                 (struct sockaddr *)&cliaddr, &len);

        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, "exit") == 0)
        {
            printf("[SERVER] Client requested exit\n");
            break;
        }

        printf("\n[SERVER INPUT] Received string: %s\n", buffer);

        permute(buffer, 0, strlen(buffer) - 1, result);

        printf("[SERVER OUTPUT] Sending permutations\n");

        sendto(sockfd, result, strlen(result), 0,
               (struct sockaddr *)&cliaddr, len);
    }

    close(sockfd);
    return 0;
}
