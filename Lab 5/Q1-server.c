#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 8080

struct client_info
{
    int sock;
    struct sockaddr_in addr;
};

char messages[2][50];
struct sockaddr_in addrs[2];
int count = 0;

pthread_mutex_t lock;

void *handle_client(void *arg)
{
    struct client_info *ci = (struct client_info *)arg;
    char buffer[50];

    recv(ci->sock, buffer, sizeof(buffer), 0);

    pthread_mutex_lock(&lock);

    strcpy(messages[count], buffer);
    addrs[count] = ci->addr;
    count++;

    printf("[SERVER] Received from %s:%d → %s\n",
           inet_ntoa(ci->addr.sin_addr),
           ntohs(ci->addr.sin_port),
           buffer);

    pthread_mutex_unlock(&lock);

    close(ci->sock);
    free(ci);
    return NULL;
}

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in serv, cli;
    socklen_t len = sizeof(cli);
    pthread_t tid;

    pthread_mutex_init(&lock, NULL);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&serv, sizeof(serv));
    listen(server_fd, 5);

    printf("Server running...\n");

    for (int i = 0; i < 2; i++)
    {
        client_fd = accept(server_fd,
                           (struct sockaddr *)&cli, &len);

        struct client_info *ci = malloc(sizeof(*ci));
        ci->sock = client_fd;
        ci->addr = cli;

        pthread_create(&tid, NULL, handle_client, ci);
        pthread_join(tid, NULL);
    }

    FILE *fp = fopen("data.txt", "r");
    char base[50];
    fscanf(fp, "%s", base);
    fclose(fp);

    printf("\n[SERVER OUTPUT]\n");
    printf("%s %s %s\n", base, messages[0], messages[1]);

    printf("Client1 Address: %s:%d\n",
           inet_ntoa(addrs[0].sin_addr),
           ntohs(addrs[0].sin_port));

    printf("Client2 Address: %s:%d\n",
           inet_ntoa(addrs[1].sin_addr),
           ntohs(addrs[1].sin_port));

    close(server_fd);
    return 0;
}
