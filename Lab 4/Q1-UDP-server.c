#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORT 9090
#define MAX 1024

int main()
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    int option;
    char buffer[MAX];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    printf("UDP Server waiting...\n");

    len = sizeof(cliaddr);

    while (1)
    {
        recvfrom(sockfd, &option, sizeof(option), 0,
                 (struct sockaddr *)&cliaddr, &len);

        if (option == 4)
            break;

        pid_t pid = fork();

        if (pid == 0)
        {
            recvfrom(sockfd, buffer, MAX, 0,
                     (struct sockaddr *)&cliaddr, &len);

            printf("[SERVER] Option %d received: %s\n", option, buffer);

            if (option == 1)
                snprintf(buffer, MAX,
                         "PID: %d\nName: Richa Shanbhag\nAddress: Manipal\n", getpid());
            else if (option == 2)
                snprintf(buffer, MAX,
                         "PID: %d\nDept: CCE\nSemester: 6\nCourses: CN, OS\n", getpid());
            else if (option == 3)
                snprintf(buffer, MAX,
                         "PID: %d\nMarks: 88\n", getpid());

            sendto(sockfd, buffer, strlen(buffer) + 1, 0,
                   (struct sockaddr *)&cliaddr, len);
            exit(0);
        }
        else
        {
            wait(NULL);
            printf("[SERVER] Child responded\n");
        }
    }

    close(sockfd);
    return 0;
}
