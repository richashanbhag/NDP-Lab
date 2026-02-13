#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>  
#include <time.h>

void inject_error(char data[]) {
    srand(time(0)); 
    int len = strlen(data);
    int bit_to_flip = rand() % len; 
    
    data[bit_to_flip] = (data[bit_to_flip] == '0') ? '1' : '0';
    
    printf("--- ERROR INJECTED! Bit at position %d was flipped. ---\n", bit_to_flip);
}

int calculate_parity(char binary[]) {
    int count = 0;
    for (int i = 0; binary[i] != '\0'; i++) {
        if (binary[i] == '1') count++;
    }
    return (count % 2);
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char data[64], transmitted_data[65];
    int choice, actual_parity;
    char parity_bit;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed\n");
        return -1;
    }

    printf("Enter binary data: ");
    scanf("%s", data);
    printf("Select Parity (1 for Even, 2 for Odd): ");
    scanf("%d", &choice);

    actual_parity = calculate_parity(data);
    if (choice == 1) 
        parity_bit = (actual_parity == 0) ? '0' : '1';
    else 
        parity_bit = (actual_parity == 0) ? '1' : '0';

    sprintf(transmitted_data, "%s%c", data, parity_bit);

    
    send(sock, &choice, sizeof(choice), 0);
    //inject_error(transmitted_data);
    send(sock, transmitted_data, strlen(transmitted_data), 0);

    printf("Data sent to Receiver: %s\n", transmitted_data);

    close(sock);
    return 0;
}
