#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 8080
#define MAX 2048

void sort_digits(char *str, char *result)
{
    char digits[MAX];
    int k = 0;

    for (int i = 0; str[i]; i++)
    {
        if (isdigit(str[i]))
            digits[k++] = str[i];
    }
    digits[k] = '\0';

    for (int i = 0; i < k - 1; i++)
    {
        for (int j = i + 1; j < k; j++)
        {
            if (digits[i] > digits[j])
            {
                char t = digits[i];
                digits[i] = digits[j];
                digits[j] = t;
            }
        }
    }
    strcpy(result, digits);
}

void sort_chars(char *str, char *result)
{
    char chars[MAX];
    int k = 0;

    for (int i = 0; str[i]; i++)
    {
        if (isalpha(str[i]))
            chars[k++] = str[i];
    }
    chars[k] = '\0';

    for (int i = 0; i < k - 1; i++)
    {
        for (int j = i + 1; j < k; j++)
        {
            if (chars[i] < chars[j])
            {
                char t = chars[i];
                chars[i] = chars[j];
                chars[j] = t;
            }
        }
    }
    strcpy(result, chars);
}

int main()
{
    int server_fd, new_sock;
    struct sockaddr_in serv_addr;
    char buffer[MAX];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("socket");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(server_fd, 1);

    printf("Server waiting for client...\n");
    new_sock = accept(server_fd, NULL, NULL);

    read(new_sock, buffer, MAX);
    printf("Received string: %s\n", buffer);

    pid_t pid = fork();

    if (pid == 0)
    {
        char result[MAX], msg[MAX];
        sort_digits(buffer, result);

        snprintf(msg, MAX,
                 "Child Process\nPID: %d\nSorted Digits (Ascending): %.1800s\n",
                 getpid(), result);
        write(new_sock, msg, strlen(msg));
    }

    else
    {
        char result[MAX], msg[MAX];
        sort_chars(buffer, result);

        snprintf(msg, MAX,
                 "Parent Process\nPID: %d\nSorted Characters (Descending): %.1800s\n",
                 getpid(), result);

        write(new_sock, msg, strlen(msg));
    }

    close(new_sock);
    close(server_fd);
    return 0;
}
