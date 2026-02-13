#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 100

int main()
{
    int sockfd;
    struct sockaddr_in serv_addr;
    char str1[MAX], str2[MAX], buffer[MAX];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    printf("Enter first string: ");
    scanf("%s", str1);

    printf("Enter second string: ");
    scanf("%s", str2);

    send(sockfd, str1, strlen(str1) + 1, 0);
    send(sockfd, str2, strlen(str2) + 1, 0);

    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("Server Response: %s\n", buffer);

    close(sockfd);
    return 0;
}
