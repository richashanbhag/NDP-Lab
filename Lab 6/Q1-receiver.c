#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int calculate_parity(char binary[]) {
    int count = 0;
    for (int i = 0; binary[i] != '\0'; i++) {
        if (binary[i] == '1') count++;
    }
    return (count % 2); 
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    int choice;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Server (Receiver) is waiting for data...\n");
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    
    recv(new_socket, &choice, sizeof(choice), 0);
    
    read(new_socket, buffer, 1024);

    printf("\nData Received: %s\n", buffer);
    int check = calculate_parity(buffer);

    if (choice == 1) { 
        if (check == 0) printf("Result: No error detected (Even parity OK).\n");
        else printf("Result: Error detected! (Data corrupt).\n");
    } else { 
        if (check != 0) printf("Result: No error detected (Odd parity OK).\n");
        else printf("Result: Error detected! (Data corrupt).\n");
    }

    close(new_socket);
    close(server_fd);
    return 0;
}