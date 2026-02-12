#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

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
    int server_fd, client_fd, choice;
    struct sockaddr_in addr;
    struct Book b;
    FILE *fp;
    char search[50];
    int found;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 1);

    printf(" Book Database Server Started...\n");
    client_fd = accept(server_fd, NULL, NULL);
    printf(" Client Connected\n");

    while (1)
    {
        recv(client_fd, &choice, sizeof(choice), 0);
        printf("\n[SERVER] Option received: %d\n", choice);

        if (choice == 1)
        {
            recv(client_fd, &b, sizeof(b), 0);

            fp = fopen("books.dat", "ab");
            fwrite(&b, sizeof(b), 1, fp);
            fclose(fp);

            printf("[SERVER] Inserted Book: %s\n", b.title);
            send(client_fd, "Book inserted successfully", 27, 0);
        }

        else if (choice == 2)
        {
            int acc, deleted = 0;
            recv(client_fd, &acc, sizeof(acc), 0);

            fp = fopen("books.dat", "rb");
            FILE *temp = fopen("temp.dat", "wb");

            while (fread(&b, sizeof(b), 1, fp))
            {
                if (b.acc_no != acc)
                    fwrite(&b, sizeof(b), 1, temp);
                else
                    deleted = 1;
            }

            fclose(fp);
            fclose(temp);

            remove("books.dat");
            rename("temp.dat", "books.dat");

            if (deleted)
                send(client_fd, "Book deleted successfully", 26, 0);
            else
                send(client_fd, "Book not found", 15, 0);

            printf("[SERVER] Delete processed\n");
        }

        else if (choice == 3)
        {
            fp = fopen("books.dat", "rb");

            while (fread(&b, sizeof(b), 1, fp))
                send(client_fd, &b, sizeof(b), 0);

            fclose(fp);

            b.acc_no = -1;
            send(client_fd, &b, sizeof(b), 0);

            printf("[SERVER] Displayed all books\n");
        }

        else if (choice == 4)
        {
            int type;
            recv(client_fd, &type, sizeof(type), 0);
            recv(client_fd, search, sizeof(search), 0);

            fp = fopen("books.dat", "rb");
            found = 0;

            while (fread(&b, sizeof(b), 1, fp))
            {
                if ((type == 1 && strcmp(b.title, search) == 0) ||
                    (type == 2 && strcmp(b.author, search) == 0))
                {
                    send(client_fd, &b, sizeof(b), 0);
                    found = 1;
                }
            }

            fclose(fp);
            b.acc_no = -1;
            send(client_fd, &b, sizeof(b), 0);

            if (!found)
                printf("[SERVER] No matching book found\n");
            else
                printf("[SERVER] Search successful\n");
        }

        else if (choice == 5)
        {
            printf("[SERVER] Client exited\n");
            break;
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
