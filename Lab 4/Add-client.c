#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

struct Book
{
    char title[50];
    char author[50];
    int acc_no;
    int pages;
    char publisher[50];
};

int main()
{
    int sockfd, choice, type;
    struct sockaddr_in servaddr;
    struct Book b;
    char msg[100], search[50];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    while (1)
    {
        printf("\n1.Insert\n2.Delete\n3.Display\n4.Search\n5.Exit\n");
        scanf("%d", &choice);
        send(sockfd, &choice, sizeof(choice), 0);

        if (choice == 1)
        {
            getchar();

            printf("Title: ");
            fgets(b.title, 50, stdin);
            b.title[strcspn(b.title, "\n")] = '\0';

            printf("Author: ");
            fgets(b.author, 50, stdin);
            b.author[strcspn(b.author, "\n")] = '\0';

            printf("Accession No: ");
            scanf("%d", &b.acc_no);

            printf("Pages: ");
            scanf("%d", &b.pages);

            getchar();
            printf("Publisher: ");
            fgets(b.publisher, 50, stdin);
            b.publisher[strcspn(b.publisher, "\n")] = '\0';

            send(sockfd, &b, sizeof(b), 0);

            memset(msg, 0, sizeof(msg));
            recv(sockfd, msg, sizeof(msg), 0);
            printf("[CLIENT] %s\n", msg);
        }

        else if (choice == 2)
        {
            int acc;
            printf("Enter Accession No: ");
            scanf("%d", &acc);

            send(sockfd, &acc, sizeof(acc), 0);

            memset(msg, 0, sizeof(msg));
            recv(sockfd, msg, sizeof(msg), 0);
            printf("[CLIENT] %s\n", msg);
        }

        else if (choice == 3)
        {
            printf("\n--- Book List ---\n");
            while (1)
            {
                recv(sockfd, &b, sizeof(b), 0);
                if (b.acc_no == -1)
                    break;

                printf("%s | %s | %d | %d | %s\n",
                       b.title, b.author, b.acc_no, b.pages, b.publisher);
            }
        }

        else if (choice == 4)
        {
            printf("Search by 1.Title 2.Author: ");
            scanf("%d", &type);
            send(sockfd, &type, sizeof(type), 0);

            getchar();
            printf("Enter search text: ");
            fgets(search, 50, stdin);
            search[strcspn(search, "\n")] = '\0';

            send(sockfd, search, sizeof(search), 0);

            printf("\n--- Search Results ---\n");
            while (1)
            {
                recv(sockfd, &b, sizeof(b), 0);
                if (b.acc_no == -1)
                    break;

                printf("%s | %s | %d | %d | %s\n",
                       b.title, b.author, b.acc_no, b.pages, b.publisher);
            }
        }

        else if (choice == 5)
        {
            printf("Exiting...\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}
