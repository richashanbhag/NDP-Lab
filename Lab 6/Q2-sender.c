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

void calculate_crc(char *data, char *gen, char *rem) {
    int n = strlen(data), g = strlen(gen);
    char temp[256];
    strcpy(temp, data);
    
    for (int i = 0; i < g - 1; i++) temp[n + i] = '0';
    temp[n + g - 1] = '\0';

    for (int i = 0; i < n; i++) {
        if (temp[i] == '1') xor_bits(&temp[i], gen);
    }
    strncpy(rem, &temp[n], g - 1);
    rem[g - 1] = '\0';
}

int main() {
    int sock, choice;
    struct sockaddr_in serv_addr;
    char data[128], gen[32], rem[32], final_msg[160];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8081);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed. Make sure the Server is running.\n");
        return -1;
    }

    while (1) {
        printf("\n--- CRC SENDER MENU ---\n");
        printf("1. CRC-12\n2. CRC-16\n3. CRC-CCITT\n4. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        if (choice == 4) {
            int exit_signal = 4;
            send(sock, &exit_signal, sizeof(int), 0);
            break;
        }

        if (choice < 1 || choice > 3) {
            printf("Invalid choice. Try again.\n");
            continue;
        }

        printf("Enter binary data: ");
        scanf("%s", data);

        if (choice == 1) strcpy(gen, CRC12);
        else if (choice == 2) strcpy(gen, CRC16);
        else strcpy(gen, CRCCCITT);

        calculate_crc(data, gen, rem);
        sprintf(final_msg, "%s%s", data, rem);

        
        send(sock, &choice, sizeof(int), 0);
        send(sock, final_msg, strlen(final_msg), 0);
        
        printf("Transmission successful.\nSent string (Data+CRC): %s\n", final_msg);
        printf("---\n");
    }

    close(sock);
    printf("Sender closed.\n");
    return 0;
}