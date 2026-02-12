#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF 4096

int main()
{
    int sock;
    struct sockaddr_in serv;
    char filename[100], buffer[BUF];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&serv, sizeof(serv));

    while (1)
    {
        printf("\nEnter filename (or stop): ");
        scanf("%s", filename);

        send(sock, filename, sizeof(filename), 0);

        if (strcmp(filename, "stop") == 0)
            break;

        recv(sock, buffer, sizeof(buffer), 0);
        printf("\n[CLIENT OUTPUT]\n%s\n", buffer);
    }

    close(sock);
    return 0;
}
