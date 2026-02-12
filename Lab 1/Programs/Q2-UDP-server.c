#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

#define MAXSIZE 200

int main()
{
    int sockfd;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t len;
    char buff[MAXSIZE], result[MAXSIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        printf("Socket Creation Error\n");
        return 0;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3399);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    printf("UDP Server Ready...\n");

    len = sizeof(clientaddr);

    while (1)
    {
        memset(buff, 0, sizeof(buff));

        recvfrom(sockfd, buff, sizeof(buff), 0,
                 (struct sockaddr *)&clientaddr, &len);

        if (strcmp(buff, "Halt") == 0)
        {
            printf("Client halted.\n");
            break;
        }

        printf("\nServer Received: %s\n", buff);

        int i, a = 0, e = 0, ii = 0, o = 0, u = 0;
        int length = strlen(buff);
        int pal = 1;

        for (i = 0; i < length / 2; i++)
            if (buff[i] != buff[length - 1 - i])
                pal = 0;

        for (i = 0; i < length; i++)
        {
            if (buff[i] == 'a')
                a++;
            else if (buff[i] == 'e')
                e++;
            else if (buff[i] == 'i')
                ii++;
            else if (buff[i] == 'o')
                o++;
            else if (buff[i] == 'u')
                u++;
        }

        sprintf(result,
                "Length=%d | Palindrome=%s | a=%d e=%d i=%d o=%d u=%d",
                length, pal ? "Yes" : "No", a, e, ii, o, u);

        printf("Server Output: %s\n", result);

        sendto(sockfd, result, strlen(result) + 1, 0,
               (struct sockaddr *)&clientaddr, len);
    }

    close(sockfd);
    return 0;
}
