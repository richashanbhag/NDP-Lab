#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main()
{
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[MAX];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    printf("Enter alphanumeric string: ");
    fgets(buffer, MAX, stdin);

    write(sock, buffer, strlen(buffer));

    printf("\nResults from server:\n");

    while (1)
    {
        memset(buffer, 0, MAX);

        if (read(sock, buffer, MAX) <= 0)
            break;

        printf("%s\n", buffer);
    }

    close(sock);
    return 0;
}
