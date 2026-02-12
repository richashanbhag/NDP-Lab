#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

void sortArray(int arr[], int n, int ascending)
{
    int temp;
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if ((ascending && arr[i] > arr[j]) ||
                (!ascending && arr[i] < arr[j]))
            {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[MAX];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("Socket failed");
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Server listening on port %d...\n", PORT);

    new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
    printf("Client connected!\n");

    while (1)
    {
        int n, choice, num;
        int arr[100];
        char response[MAX];
        response[0] = '\0';

        read(new_socket, &n, sizeof(int));
        if (n == 0)
        {
            printf("Client exited.\n");
            break;
        }

        read(new_socket, arr, n * sizeof(int));
        read(new_socket, &choice, sizeof(int));

        printf("\n--- Server Received Data ---\n");
        printf("Number of elements: %d\n", n);

        printf("Array elements: ");
        for (int i = 0; i < n; i++)
            printf("%d ", arr[i]);
        printf("\n");

        printf("Choice received: %d\n", choice);

        if (choice == 1)
        {
            read(new_socket, &num, sizeof(int));
            printf("Search key received: %d\n", num);

            int foundIndex = -1;
            for (int i = 0; i < n; i++)
            {
                if (arr[i] == num)
                {
                    foundIndex = i;
                    break;
                }
            }

            if (foundIndex != -1)
                sprintf(response, "Number %d found at index %d", num, foundIndex);
            else
                sprintf(response, "Number %d not found in array", num);
        }

        else if (choice == 2)
        {
            sortArray(arr, n, 1);
            sprintf(response, "Sorted Ascending: ");
            for (int i = 0; i < n; i++)
                sprintf(response + strlen(response), "%d ", arr[i]);
        }

        else if (choice == 3)
        {
            sortArray(arr, n, 0);
            sprintf(response, "Sorted Descending: ");
            for (int i = 0; i < n; i++)
                sprintf(response + strlen(response), "%d ", arr[i]);
        }

        else if (choice == 4)
        {
            sprintf(response, "Even: ");
            for (int i = 0; i < n; i++)
                if (arr[i] % 2 == 0)
                    sprintf(response + strlen(response), "%d ", arr[i]);

            strcat(response, "| Odd: ");
            for (int i = 0; i < n; i++)
                if (arr[i] % 2 != 0)
                    sprintf(response + strlen(response), "%d ", arr[i]);
        }

        printf("Server Output: %s\n", response);

        write(new_socket, response, strlen(response) + 1);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
