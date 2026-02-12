#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define MAXSIZE 100

int main()
{
    int sockfd, retval;
    int sentbytes, recedbytes;
    struct sockaddr_in serveraddr;
    char buff[MAXSIZE];
    int n, arr[20], choice, num, i;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Socket Creation Error\n");
        return 0;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    retval = connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1)
    {
        printf("Connection Error\n");
        return 0;
    }

    while (1)
    {
        printf("\nEnter number of elements (0 to exit): ");
        scanf("%d", &n);

        send(sockfd, &n, sizeof(int), 0);

        if (n == 0)
            break;

        printf("Enter %d elements: ", n);
        for (i = 0; i < n; i++)
            scanf("%d", &arr[i]);

        printf("\n1.Search\n2.Sort Ascending\n3.Sort Descending\n4.Split Odd/Even\n5.Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        send(sockfd, arr, n * sizeof(int), 0);
        send(sockfd, &choice, sizeof(int), 0);

        if (choice == 1)
        {
            printf("Enter number to search: ");
            scanf("%d", &num);
            send(sockfd, &num, sizeof(int), 0);
        }

        recedbytes = recv(sockfd, buff, sizeof(buff), 0);
        buff[recedbytes] = '\0';

        printf("Server Output: %s\n", buff);

        if (choice == 5)
            break;
    }

    close(sockfd);
    return 0;
}
