#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main()
{
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[BUF_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("connect");
        exit(1);
    }

    printf("\n[Client Parent] PID=%d PPID=%d\n", getpid(), getppid());

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }

    if (pid == 0)
    {
        printf("[Client Child] PID=%d PPID=%d\n", getpid(), getppid());
        while (1)
        {
            memset(buffer, 0, BUF_SIZE);
            if (read(sock, buffer, BUF_SIZE) <= 0)
                break;
            printf("\nServer: %s", buffer);
        }
    }

    else
    {
        while (1)
        {
            memset(buffer, 0, BUF_SIZE);
            fgets(buffer, BUF_SIZE, stdin);
            write(sock, buffer, strlen(buffer));
        }
    }

    close(sock);
    return 0;
}
