#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define CRC12 "1100000001111"
#define CRC16 "11000000000000101"
#define CRCCCITT "10001000000100001"

void xor_bits(char *temp, char *gen) {
    for (int j = 1; j < strlen(gen); j++)
        temp[j] = (temp[j] == gen[j]) ? '0' : '1';
}

int check_error(char *msg, char *gen) {
    int n = strlen(msg), g = strlen(gen);
    char temp[160];
    strcpy(temp, msg);

    for (int i = 0; i <= n - g; i++) {
        if (temp[i] == '1') xor_bits(&temp[i], gen);
    }

    for (int i = n - g + 1; i < n; i++) {
        if (temp[i] == '1') return 1; 
    }
    return 0; 
}

int main() {
    int server_fd, new_sock, choice;
    struct sockaddr_in addr;
    char buffer[160] = {0}, gen[32];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8081);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 3);
    
    printf("Receiver waiting for CRC data...\n");
    new_sock = accept(server_fd, NULL, NULL);

    
while (1) {
    
    int status = recv(new_sock, &choice, sizeof(int), 0);
    if (status <= 0 || choice == 4) {
        printf("Client disconnected or exited.\n");
        break;
    }

    
    memset(buffer, 0, sizeof(buffer));
    read(new_sock, buffer, 160);

    if (choice == 1) strcpy(gen, CRC12);
    else if (choice == 2) strcpy(gen, CRC16);
    else if (choice == 3) strcpy(gen, CRCCCITT);

    printf("\nReceived Data: %s\n", buffer);
    if (check_error(buffer, gen)) 
        printf("RESULT: Error Detected!\n");
    else 
        printf("RESULT: No Error detected (CRC match).\n");
}
    close(new_sock); close(server_fd);
    return 0;
}