#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in addr;
    int choice;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 1);

    printf(" Calculator Server waiting for client...\n");
    client_fd = accept(server_fd, NULL, NULL);
    printf(" Client connected\n");

    while (1)
    {
        recv(client_fd, &choice, sizeof(choice), 0);
        printf("\n[SERVER] Option received: %d\n", choice);

        if (choice == 1)
        {
            int a, b, add, sub;

            recv(client_fd, &a, sizeof(a), 0);
            recv(client_fd, &b, sizeof(b), 0);

            printf("[SERVER] Received numbers: %d and %d\n", a, b);

            add = a + b;
            sub = a - b;

            printf("[SERVER] Addition = %d, Subtraction = %d\n", add, sub);

            send(client_fd, &add, sizeof(add), 0);
            send(client_fd, &sub, sizeof(sub), 0);
        }

        else if (choice == 2)
        {
            float a, b, x;

            recv(client_fd, &a, sizeof(a), 0);
            recv(client_fd, &b, sizeof(b), 0);

            printf("[SERVER] Linear equation: %.2fx + %.2f = 0\n", a, b);

            x = -b / a;

            printf("[SERVER] Computed x = %.2f\n", x);
            send(client_fd, &x, sizeof(x), 0);
        }

        else if (choice == 3)
        {
            int r1, c1, r2, c2;
            int A[10][10], B[10][10], C[10][10];

            recv(client_fd, &r1, sizeof(r1), 0);
            recv(client_fd, &c1, sizeof(c1), 0);
            recv(client_fd, &r2, sizeof(r2), 0);
            recv(client_fd, &c2, sizeof(c2), 0);

            printf("[SERVER] Matrix sizes: A(%dx%d), B(%dx%d)\n", r1, c1, r2, c2);

            for (int i = 0; i < r1; i++)
                for (int j = 0; j < c1; j++)
                    recv(client_fd, &A[i][j], sizeof(int), 0);

            for (int i = 0; i < r2; i++)
                for (int j = 0; j < c2; j++)
                    recv(client_fd, &B[i][j], sizeof(int), 0);

            printf("[SERVER] Performing matrix multiplication...\n");

            for (int i = 0; i < r1; i++)
                for (int j = 0; j < c2; j++)
                {
                    C[i][j] = 0;
                    for (int k = 0; k < c1; k++)
                        C[i][j] += A[i][k] * B[k][j];
                }

            printf("[SERVER] Result matrix computed\n");

            for (int i = 0; i < r1; i++)
                for (int j = 0; j < c2; j++)
                    send(client_fd, &C[i][j], sizeof(int), 0);
        }

        else if (choice == 4)
        {
            printf("[SERVER] Client exited\n");
            break;
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
