#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

#define MAXSIZE 100

int main()
{
    int sockfd, newsockfd, retval;
    socklen_t actuallen;
    int sentbytes, recedbytes;
    struct sockaddr_in serveraddr, clientaddr;
    char buff[MAXSIZE];
    int n, arr[20], choice, num, i, j;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Socket Creation Error\n");
        return 0;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    retval = bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1)
    {
        printf("Binding Error\n");
        return 0;
    }

    listen(sockfd, 1);
    printf("Server Waiting...\n");

    actuallen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &actuallen);

    while (1)
    {
        recv(newsockfd, &n, sizeof(int), 0);

        if (n == 0)
        {
            printf("Client exited\n");
            break;
        }

        recv(newsockfd, arr, n * sizeof(int), 0);
        recv(newsockfd, &choice, sizeof(int), 0);

        printf("\n--- Server Received ---\n");
        printf("Elements: ");
        for (i = 0; i < n; i++)
            printf("%d ", arr[i]);
        printf("\nChoice: %d\n", choice);

        buff[0] = '\0';

        if (choice == 1)
        {
            recv(newsockfd, &num, sizeof(int), 0);
            printf("Search Key: %d\n", num);

            int index = -1;
            for (i = 0; i < n; i++)
                if (arr[i] == num)
                {
                    index = i;
                    break;
                }

            if (index != -1)
                sprintf(buff, "Number %d found at index %d", num, index);
            else
                sprintf(buff, "Number %d not found", num);
        }

        else if (choice == 2)
        {
            for (i = 0; i < n - 1; i++)
                for (j = i + 1; j < n; j++)
                    if (arr[i] > arr[j])
                    {
                        int t = arr[i];
                        arr[i] = arr[j];
                        arr[j] = t;
                    }

            strcpy(buff, "Ascending Order: ");
            for (i = 0; i < n; i++)
            {
                char temp[10];
                sprintf(temp, "%d ", arr[i]);
                strcat(buff, temp);
            }
        }

        else if (choice == 3)
        {
            for (i = 0; i < n - 1; i++)
                for (j = i + 1; j < n; j++)
                    if (arr[i] < arr[j])
                    {
                        int t = arr[i];
                        arr[i] = arr[j];
                        arr[j] = t;
                    }

            strcpy(buff, "Descending Order: ");
            for (i = 0; i < n; i++)
            {
                char temp[10];
                sprintf(temp, "%d ", arr[i]);
                strcat(buff, temp);
            }
        }

        else if (choice == 4)
        {
            strcpy(buff, "Even: ");
            for (i = 0; i < n; i++)
                if (arr[i] % 2 == 0)
                {
                    char temp[10];
                    sprintf(temp, "%d ", arr[i]);
                    strcat(buff, temp);
                }

            strcat(buff, "| Odd: ");
            for (i = 0; i < n; i++)
                if (arr[i] % 2 != 0)
                {
                    char temp[10];
                    sprintf(temp, "%d ", arr[i]);
                    strcat(buff, temp);
                }
        }

        printf("Server Output: %s\n", buff);
        send(newsockfd, buff, sizeof(buff), 0);
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
