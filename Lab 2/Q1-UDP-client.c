#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF 1024

int main()
{
    int sock;
    struct sockaddr_in serv;
    socklen_t len;
    char buffer[BUF], filename[100];

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");

    len = sizeof(serv);

    printf("Enter filename: ");
    scanf("%s", filename);

    sendto(sock, filename, sizeof(filename), 0,
           (struct sockaddr *)&serv, len);

    recvfrom(sock, buffer, BUF, 0,
             (struct sockaddr *)&serv, &len);

    printf("[CLIENT RECEIVED] %s\n", buffer);

    if (strcmp(buffer, "File OK") != 0)
    {
        close(sock);
        return 0;
    }

    while (1)
    {
        int opt;
        printf("\n1.Search 2.Replace 3.Reorder 4.Exit\n");
        scanf("%d", &opt);

        sendto(sock, &opt, sizeof(opt), 0,
               (struct sockaddr *)&serv, len);

        if (opt == 1)
        {
            printf("Enter string: ");
            scanf("%s", buffer);
            sendto(sock, buffer, sizeof(buffer), 0,
                   (struct sockaddr *)&serv, len);

            recvfrom(sock, buffer, BUF, 0,
                     (struct sockaddr *)&serv, &len);
            printf("[CLIENT RECEIVED] %s\n", buffer);
        }

        else if (opt == 2)
        {
            char s1[50], s2[50];

            printf("Enter string to replace: ");
            scanf("%s", s1);
            printf("Enter new string: ");
            scanf("%s", s2);

            sendto(sock, s1, sizeof(s1), 0,
                   (struct sockaddr *)&serv, len);
            sendto(sock, s2, sizeof(s2), 0,
                   (struct sockaddr *)&serv, len);

            recvfrom(sock, buffer, BUF, 0,
                     (struct sockaddr *)&serv, &len);
            printf("[CLIENT RECEIVED] %s\n", buffer);
        }

        else if (opt == 3)
        {
            recvfrom(sock, buffer, BUF, 0,
                     (struct sockaddr *)&serv, &len);
            printf("[CLIENT RECEIVED] %s\n", buffer);
        }

        else if (opt == 4)
        {
            break;
        }
    }

    close(sock);
    return 0;
}
