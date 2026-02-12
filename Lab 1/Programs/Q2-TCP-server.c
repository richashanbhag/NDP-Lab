#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

#define MAXSIZE 200

int main()
{
    int sockfd, newsockfd;
    socklen_t actuallen;
    struct sockaddr_in serveraddr, clientaddr;
    char buff[MAXSIZE], result[MAXSIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Socket Creation Error\n");
        return 0;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(sockfd, 1);

    printf("TCP Server Waiting...\n");

    actuallen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &actuallen);

    while (1)
    {
        memset(buff, 0, sizeof(buff));
        recv(newsockfd, buff, sizeof(buff), 0);

        if (strcmp(buff, "Halt") == 0)
        {
            printf("Client halted.\n");
            break;
        }

        printf("\nServer Received: %s\n", buff);

        int len = strlen(buff);
        int i, a = 0, e = 0, ii = 0, o = 0, u = 0;
        int pal = 1;

        for (i = 0; i < len / 2; i++)
            if (buff[i] != buff[len - 1 - i])
                pal = 0;

        for (i = 0; i < len; i++)
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
                len, pal ? "Yes" : "No", a, e, ii, o, u);

        printf("Server Output: %s\n", result);

        send(newsockfd, result, strlen(result) + 1, 0);
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
