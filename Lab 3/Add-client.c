#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
    int sockfd, choice;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    while (1)
    {
        printf("\n1. Add/Subtract\n2. Solve Linear Equation\n3. Matrix Multiplication\n4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        send(sockfd, &choice, sizeof(choice), 0);

        if (choice == 1)
        {
            int a, b, add, sub;

            printf("Enter two integers: ");
            scanf("%d %d", &a, &b);

            send(sockfd, &a, sizeof(a), 0);
            send(sockfd, &b, sizeof(b), 0);

            recv(sockfd, &add, sizeof(add), 0);
            recv(sockfd, &sub, sizeof(sub), 0);

            printf("Addition: %d\nSubtraction: %d\n", add, sub);
        }

        else if (choice == 2)
        {
            float a, b, x;

            printf("Enter a and b for ax + b = 0: ");
            scanf("%f %f", &a, &b);

            send(sockfd, &a, sizeof(a), 0);
            send(sockfd, &b, sizeof(b), 0);

            recv(sockfd, &x, sizeof(x), 0);
            printf("Value of x = %.2f\n", x);
        }

        else if (choice == 3)
        {
            int r1, c1, r2, c2;
            int A[10][10], B[10][10], C[10][10];

            printf("Enter rows and columns of matrix A: ");
            scanf("%d %d", &r1, &c1);
            printf("Enter rows and columns of matrix B: ");
            scanf("%d %d", &r2, &c2);

            send(sockfd, &r1, sizeof(r1), 0);
            send(sockfd, &c1, sizeof(c1), 0);
            send(sockfd, &r2, sizeof(r2), 0);
            send(sockfd, &c2, sizeof(c2), 0);

            printf("Enter matrix A:\n");
            for (int i = 0; i < r1; i++)
                for (int j = 0; j < c1; j++)
                {
                    scanf("%d", &A[i][j]);
                    send(sockfd, &A[i][j], sizeof(int), 0);
                }

            printf("Enter matrix B:\n");
            for (int i = 0; i < r2; i++)
                for (int j = 0; j < c2; j++)
                {
                    scanf("%d", &B[i][j]);
                    send(sockfd, &B[i][j], sizeof(int), 0);
                }

            printf("Result matrix:\n");
            for (int i = 0; i < r1; i++)
            {
                for (int j = 0; j < c2; j++)
                {
                    recv(sockfd, &C[i][j], sizeof(int), 0);
                    printf("%d ", C[i][j]);
                }
                printf("\n");
            }
        }

        else if (choice == 4)
        {
            break;
        }
    }

    close(sockfd);
    return 0;
}
