#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 8081
#define BUF 4096

int main()
{
    int sock;
    struct sockaddr_in serv, cli;
    socklen_t len;
    char filename[100], buffer[BUF];

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    serv.sin_addr.s_addr = INADDR_ANY;

    bind(sock, (struct sockaddr *)&serv, sizeof(serv));

    printf("UDP File Server waiting...\n");
    len = sizeof(cli);

    while (1)
    {
        recvfrom(sock, filename, sizeof(filename), 0,
                 (struct sockaddr *)&cli, &len);

        printf("\n[SERVER INPUT] Filename received: %s\n", filename);

        if (strcmp(filename, "stop") == 0)
        {
            printf("[SERVER] Client requested termination\n");
            break;
        }

        FILE *fp = fopen(filename, "r");
        if (!fp)
        {
            strcpy(buffer, "File not present");
            sendto(sock, buffer, strlen(buffer) + 1, 0,
                   (struct sockaddr *)&cli, len);
            continue;
        }

        int size = 0, lines = 1, spaces = 0, digits = 0, alphabets = 0, others = 0;
        char ch;
        char content[3000] = "";

        while ((ch = fgetc(fp)) != EOF)
        {
            size++;
            strncat(content, &ch, 1);

            if (isalpha(ch))
                alphabets++;
            else if (isdigit(ch))
                digits++;
            else if (ch == ' ')
                spaces++;
            else if (ch == '\n')
                lines++;
            else
                others++;
        }
        fclose(fp);

        sprintf(buffer,
                "File Size: %d bytes\nLines: %d\nAlphabets: %d\nDigits: %d\nSpaces: %d\nOther Characters: %d\n\nFile Content:\n%s",
                size, lines, alphabets, digits, spaces, others, content);

        printf("[SERVER OUTPUT] File processed and sent\n");
        sendto(sock, buffer, strlen(buffer) + 1, 0,
               (struct sockaddr *)&cli, len);
    }

    close(sock);
    return 0;
}
