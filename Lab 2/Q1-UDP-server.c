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
    int sock;
    struct sockaddr_in serv, cli;
    socklen_t len;
    char buffer[BUF], filename[100];

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    serv.sin_addr.s_addr = INADDR_ANY;

    bind(sock, (struct sockaddr *)&serv, sizeof(serv));

    printf("UDP Server waiting...\n");

    len = sizeof(cli);

    recvfrom(sock, filename, sizeof(filename), 0,
             (struct sockaddr *)&cli, &len);

    printf("\n[SERVER INPUT] File requested: %s\n", filename);

    FILE *fp = fopen(filename, "r+");
    if (!fp)
    {
        strcpy(buffer, "File not present");
        sendto(sock, buffer, strlen(buffer) + 1, 0,
               (struct sockaddr *)&cli, len);
        return 0;
    }

    strcpy(buffer, "File OK");
    sendto(sock, buffer, strlen(buffer) + 1, 0,
           (struct sockaddr *)&cli, len);

    while (1)
    {
        int opt;
        recvfrom(sock, &opt, sizeof(opt), 0,
                 (struct sockaddr *)&cli, &len);

        printf("\n[SERVER INPUT] Option: %d\n", opt);

        if (opt == 1)
        {
            char str[100];
            recvfrom(sock, str, sizeof(str), 0,
                     (struct sockaddr *)&cli, &len);

            printf("[SERVER INPUT] Search string: %s\n", str);

            int count = 0;
            char line[BUF];
            rewind(fp);
            while (fgets(line, BUF, fp))
                if (strstr(line, str))
                    count++;

            if (count)
                sprintf(buffer, "String found %d times", count);
            else
                strcpy(buffer, "String not found");

            printf("[SERVER OUTPUT] %s\n", buffer);

            sendto(sock, buffer, strlen(buffer) + 1, 0,
                   (struct sockaddr *)&cli, len);
        }

        else if (opt == 2)
        {
            char s1[50], s2[50], temp[10000] = "";

            recvfrom(sock, s1, sizeof(s1), 0,
                     (struct sockaddr *)&cli, &len);
            recvfrom(sock, s2, sizeof(s2), 0,
                     (struct sockaddr *)&cli, &len);

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
                strcpy(buffer, "String replaced");
            }
            else
                strcpy(buffer, "String not found");

            printf("[SERVER OUTPUT] %s\n", buffer);

            sendto(sock, buffer, strlen(buffer) + 1, 0,
                   (struct sockaddr *)&cli, len);
        }

        else if (opt == 3)
        {
            reorder_file(filename);
            strcpy(buffer, "File reordered");
            printf("[SERVER OUTPUT] File reordered\n");

            sendto(sock, buffer, strlen(buffer) + 1, 0,
                   (struct sockaddr *)&cli, len);
        }

        else if (opt == 4)
        {
            printf("[SERVER] Client exited\n");
            break;
        }
    }

    fclose(fp);
    close(sock);
    return 0;
}
