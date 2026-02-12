#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[MAX];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        printf("Socket creation error\n");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Connection failed\n");
        return 1;
    }

    printf("Connected to server!\n");

    while (1)
    {
        int n, arr[100], choice, num;

        printf("\nEnter number of elements (0 to exit): ");
        scanf("%d", &n);
        if (n == 0)
        {
            int exitChoice = 5;
            write(sock, &n, sizeof(int));
            write(sock, arr, 0);
            write(sock, &exitChoice, sizeof(int));
            break;
        }

        printf("Enter %d elements: ", n);
        for (int i = 0; i < n; i++)
            scanf("%d", &arr[i]);

        printf("\nChoose operation:\n");
        printf("1. Search\n2. Sort Ascending\n3. Sort Descending\n4. Split Odd/Even\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        write(sock, &n, sizeof(int));
        write(sock, arr, n * sizeof(int));
        write(sock, &choice, sizeof(int));

        if (choice == 1)
        {
            printf("Enter number to search: ");
            scanf("%d", &num);
            write(sock, &num, sizeof(int));
        }

        read(sock, buffer, MAX);
        printf("Server: %s\n", buffer);

        if (choice == 5)
            break;
    }

    close(sock);
    printf("Disconnected from server.\n");
    return 0;
}
