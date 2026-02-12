#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>

#define PORT 8080
#define MAX 1024

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in addr;
    int option;
    char buffer[MAX];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 1);

    printf(" TCP Server waiting...\n");
    client_fd = accept(server_fd, NULL, NULL);
    printf("Client connected\n");

    while (1)
    {
        recv(client_fd, &option, sizeof(option), 0);
        if (option == 4)
            break;

        printf("\n[SERVER] Option received: %d\n", option);

        pid_t pid = fork();

        if (pid == 0)
        {
            memset(buffer, 0, MAX);

            if (option == 1)
            {
                recv(client_fd, buffer, MAX, 0);
                printf("[CHILD-1] Reg No received: %s\n", buffer);

                snprintf(buffer, MAX,
                         "PID: %d\nName: Richa Shanbhag\nAddress: Manipal, Karnataka\n",
                         getpid());
            }
            else if (option == 2)
            {
                recv(client_fd, buffer, MAX, 0);
                printf("[CHILD-2] Name received: %s\n", buffer);

                snprintf(buffer, MAX,
                         "PID: %d\nDept: CCE\nSemester: 6\nSection: A\nCourses: CN, OS, DBMS\n",
                         getpid());
            }
            else if (option == 3)
            {
                recv(client_fd, buffer, MAX, 0);
                printf("[CHILD-3] Subject Code received: %s\n", buffer);

                snprintf(buffer, MAX,
                         "PID: %d\nMarks Obtained: 88\n",
                         getpid());
            }

            send(client_fd, buffer, strlen(buffer) + 1, 0);
            exit(0);
        }
        else
        {
            wait(NULL);
            printf("[SERVER] Child handled request\n");
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
