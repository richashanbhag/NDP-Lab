#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF 1024

void reorder_file(const char *fname)
{
    FILE *fp = fopen(fname, "r");
    if (!fp)
        return;

    char data[10000];
    int n = fread(data, 1, sizeof(data), fp);
    fclose(fp);

    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (data[i] > data[j])
            {
                char t = data[i];
                data[i] = data[j];
                data[j] = t;
            }

    fp = fopen(fname, "w");
    fwrite(data, 1, n, fp);
    fflush(fp);
    fclose(fp);
}

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in addr;
    char buffer[BUF], filename[100];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 5);

    printf("TCP Server waiting...\n");
    client_fd = accept(server_fd, NULL, NULL);

    recv(client_fd, filename, sizeof(filename), 0);

    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        send(client_fd, "File not present", 16, 0);
        close(client_fd);
        return 0;
    }
    send(client_fd, "File OK", 7, 0);

    while (1)
    {
        int option;
        recv(client_fd, &option, sizeof(option), 0);

        if (option == 1)
        {
            char str[100];
            recv(client_fd, str, sizeof(str), 0);

            printf("[SERVER INPUT] Search string: %s\n", str);

            int count = 0;
            char line[BUF];
            rewind(fp);
            while (fgets(line, BUF, fp))
                if (strstr(line, str))
                    count++;

            if (count)
            {
                printf("[SERVER OUTPUT] String found %d times\n", count);
                send(client_fd, &count, sizeof(count), 0);
            }
            else
            {
                printf("[SERVER OUTPUT] String not found\n");
                send(client_fd, "String not found", 16, 0);
            }
        }

        else if (option == 2)
        {
            char s1[50], s2[50], temp[10000] = "";
            recv(client_fd, s1, sizeof(s1), 0);
            recv(client_fd, s2, sizeof(s2), 0);

            printf("[SERVER INPUT] Replace '%s' with '%s'\n", s1, s2);

            rewind(fp);
            char line[BUF];
            int found = 0;

            while (fgets(line, BUF, fp))
            {
                char *pos;
                while ((pos = strstr(line, s1)))
                {
                    found = 1;
                    strncpy(temp + strlen(temp), line, pos - line);
                    strcat(temp, s2);
                    strcpy(line, pos + strlen(s1));
                }
                strcat(temp, line);
            }

            if (found)
            {
                freopen(filename, "w", fp);
                fputs(temp, fp);
                fflush(fp);

                printf("[SERVER OUTPUT] String replaced successfully\n");
                send(client_fd, "String replaced", 15, 0);
            }
            else
            {
                printf("[SERVER OUTPUT] String not found\n");
                send(client_fd, "String not found", 16, 0);
            }
        }

        else if (option == 3)
        {
            reorder_file(filename);
            printf("[SERVER OUTPUT] File reordered based on ASCII\n");
            send(client_fd, "File reordered", 14, 0);
        }

        else if (option == 4)
        {
            printf("[SERVER] Client exited\n");
            break;
        }
    }

    fclose(fp);
    close(client_fd);
    close(server_fd);
    return 0;
}
