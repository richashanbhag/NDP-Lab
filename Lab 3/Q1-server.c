#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main()
{
    int server_fd, new_sock;
    struct sockaddr_in serv_addr;
    char buffer[BUF_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("socket");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("bind");
        exit(1);
    }

    listen(server_fd, 1);
    printf("Server waiting for connection...\n");

    new_sock = accept(server_fd, NULL, NULL);
    if (new_sock < 0)
    {
        perror("accept");
        exit(1);
    }

    printf("\n[Server Parent] PID=%d PPID=%d\n", getpid(), getppid());

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }

    if (pid == 0)
    {
        printf("[Server Child] PID=%d PPID=%d\n", getpid(), getppid());
        while (1)
        {
            memset(buffer, 0, BUF_SIZE);
            if (read(new_sock, buffer, BUF_SIZE) <= 0)
                break;
            printf("\nClient: %s", buffer);
        }
    }

    else
    {
        while (1)
        {
            memset(buffer, 0, BUF_SIZE);
            fgets(buffer, BUF_SIZE, stdin);
            write(new_sock, buffer, strlen(buffer));
        }
    }

    close(new_sock);
    close(server_fd);
    return 0;
}
