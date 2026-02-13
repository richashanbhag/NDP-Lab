#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[256];
    int route, seats;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("\n--- Seat Status ---\n%s\n", buffer);

    printf("Select Route (1 or 2): ");
    scanf("%d", &route);

    printf("Enter number of seats to book: ");
    scanf("%d", &seats);

    send(sockfd, &route, sizeof(int), 0);
    send(sockfd, &seats, sizeof(int), 0);

    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("\n[CLIENT] %s\n", buffer);

    close(sockfd);
    return 0;
}
