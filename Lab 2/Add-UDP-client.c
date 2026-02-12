#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUF 4096

int main()
{
    int sock;
    struct sockaddr_in serv;
    socklen_t len;
    char filename[100], buffer[BUF];

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");

    len = sizeof(serv);

    while (1)
    {
        printf("\nEnter filename (or stop): ");
        scanf("%s", filename);

        sendto(sock, filename, sizeof(filename), 0,
               (struct sockaddr *)&serv, len);

        if (strcmp(filename, "stop") == 0)
            break;

        recvfrom(sock, buffer, sizeof(buffer), 0,
                 (struct sockaddr *)&serv, &len);

        printf("\n[CLIENT OUTPUT]\n%s\n", buffer);
    }

    close(sock);
    return 0;
}
