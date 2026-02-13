#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

int total_seats[2] = {50, 40};
int booked_seats[2] = {0, 0};

pthread_mutex_t lock;

void *handle_client(void *arg)
{
    int client_fd = *(int *)arg;
    free(arg);

    int route, seats;
    char buffer[256];

    pthread_mutex_lock(&lock);

    sprintf(buffer,
            "Route 1 (A->B): Available=%d Booked=%d\n"
            "Route 2 (C->D): Available=%d Booked=%d\n",
            total_seats[0] - booked_seats[0], booked_seats[0],
            total_seats[1] - booked_seats[1], booked_seats[1]);

    pthread_mutex_unlock(&lock);

    send(client_fd, buffer, strlen(buffer) + 1, 0);

    recv(client_fd, &route, sizeof(int), 0);
    recv(client_fd, &seats, sizeof(int), 0);

    route--;

    pthread_mutex_lock(&lock);

    if (route < 0 || route > 1)
        strcpy(buffer, "Invalid route selected");
    else if (booked_seats[route] + seats <= total_seats[route])
    {
        booked_seats[route] += seats;
        sprintf(buffer,
                "Booking successful! %d seats booked on Route %d",
                seats, route + 1);
    }
    else
        strcpy(buffer, "Requested seats not available");

    printf("[SERVER] %s\n", buffer);

    pthread_mutex_unlock(&lock);

    send(client_fd, buffer, strlen(buffer) + 1, 0);

    close(client_fd);
    return NULL;
}

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    socklen_t len = sizeof(server_addr);
    pthread_t tid;

    pthread_mutex_init(&lock, NULL);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    printf("Travel Ticket Reservation Server Started...\n");

    while (1)
    {
        client_fd = accept(server_fd,
                           (struct sockaddr *)&server_addr, &len);

        int *pclient = malloc(sizeof(int));
        *pclient = client_fd;

        pthread_create(&tid, NULL, handle_client, pclient);
        pthread_detach(tid);
    }

    close(server_fd);
    return 0;
}
