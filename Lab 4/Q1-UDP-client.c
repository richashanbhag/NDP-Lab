#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define MAX 1024

int main()
{
    int sockfd, option;
    struct sockaddr_in servaddr;
    char buffer[MAX];
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    len = sizeof(servaddr);

    while (1)
    {
        printf("\n1. Registration Number\n2. Student Name\n3. Subject Code\n4. Exit\n");
        printf("Enter option: ");
        scanf("%d", &option);

        sendto(sockfd, &option, sizeof(option), 0,
               (struct sockaddr *)&servaddr, len);

        if (option == 4)
            break;

        printf("Enter value: ");
        scanf("%s", buffer);

        sendto(sockfd, buffer, strlen(buffer) + 1, 0,
               (struct sockaddr *)&servaddr, len);

        recvfrom(sockfd, buffer, MAX, 0,
                 (struct sockaddr *)&servaddr, &len);

        printf("\n[CLIENT RECEIVED]\n%s\n", buffer);
    }

    close(sockfd);
    return 0;
}
