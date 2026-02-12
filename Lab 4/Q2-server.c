#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX 1024

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in addr;
    char domain[MAX], response[MAX];
    FILE *fp;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 1);

    printf(" DNS Server running (TCP)...\n");
    client_fd = accept(server_fd, NULL, NULL);
    printf(" Client connected\n");

    while (1)
    {
        memset(domain, 0, MAX);
        recv(client_fd, domain, MAX, 0);

        if (strcmp(domain, "exit") == 0)
        {
            printf("[SERVER] Client terminated connection\n");
            break;
        }

        printf("[SERVER] DNS Query received: %s\n", domain);

        fp = fopen("database.txt", "r");
        if (fp == NULL)
        {
            strcpy(response, "DNS database not found");
        }
        else
        {
            char db_domain[100], ip[100];
            int found = 0;

            while (fscanf(fp, "%s %s", db_domain, ip) != EOF)
            {
                if (strcmp(db_domain, domain) == 0)
                {
                    sprintf(response, "IP Address: %s", ip);
                    found = 1;
                    break;
                }
            }

            if (!found)
                strcpy(response, "Domain not found");

            fclose(fp);
        }

        printf("[SERVER] Sending response: %s\n", response);
        send(client_fd, response, strlen(response) + 1, 0);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
