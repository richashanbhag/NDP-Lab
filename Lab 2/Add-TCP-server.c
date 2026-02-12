#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <ctype.h>

#define PORT 8080
#define BUF 4096

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in addr;
    char filename[100], buffer[BUF];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 1);

    printf("TCP File Server waiting...\n");
    client_fd = accept(server_fd, NULL, NULL);

    while (1)
    {
        recv(client_fd, filename, sizeof(filename), 0);
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
            send(client_fd, buffer, sizeof(buffer), 0);
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
        send(client_fd, buffer, sizeof(buffer), 0);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
