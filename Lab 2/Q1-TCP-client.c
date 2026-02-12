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
    char buffer[BUF], filename[100];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&serv, sizeof(serv));

    printf("Enter filename: ");
    scanf("%s", filename);
    send(sock, filename, sizeof(filename), 0);

    recv(sock, buffer, BUF, 0);
    if (strcmp(buffer, "File OK") != 0)
    {
        printf("File not present\n");
        close(sock);
        return 0;
    }

    while (1)
    {
        int opt;
        printf("\n1.Search 2.Replace 3.Reorder 4.Exit\n");
        scanf("%d", &opt);
        send(sock, &opt, sizeof(opt), 0);

        if (opt == 1)
        {
            char str[50];
            int count;

            printf("Enter string: ");
            scanf("%s", str);
            send(sock, str, sizeof(str), 0);

            int n = recv(sock, &count, sizeof(count), 0);

            if (n == sizeof(int))
                printf("String found %d times\n", count);
            else
            {
                recv(sock, buffer, BUF, 0);
                printf("%s\n", buffer);
            }
        }

        else if (opt == 2)
        {
            char s1[50], s2[50];

            printf("Enter string to replace: ");
            scanf("%s", s1);
            printf("Enter new string: ");
            scanf("%s", s2);

            send(sock, s1, sizeof(s1), 0);
            send(sock, s2, sizeof(s2), 0);

            recv(sock, buffer, BUF, 0);
            printf("%s\n", buffer);
        }

        else if (opt == 3)
        {
            recv(sock, buffer, BUF, 0);
            printf("%s\n", buffer);
        }

        else if (opt == 4)
        {
            break;
        }
    }

    close(sock);
    return 0;
}
