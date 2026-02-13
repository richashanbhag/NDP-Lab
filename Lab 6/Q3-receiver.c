#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int server_fd, new_sock;
    struct sockaddr_in addr;
    int code[20], r = 3, n = 7; 
    int choice, error_bit;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8082);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 3);
    
    printf("Receiver waiting for Hamming data...\n");
    new_sock = accept(server_fd, NULL, NULL);

    
    read(new_sock, code, sizeof(code));

    printf("\nReceived Data from Sender: ");
    for (int i = 1; i <= n; i++) printf("%d", code[i]);



    int syndrome = 0;
    for (int i = 0; i < r; i++) {
        int parity = 0;
        int bit_pos = (1 << i); 
        
        for (int l = 1; l <= n; l++) {
            if ((l >> i) & 1) { 
                parity ^= code[l];
            }
        }
        if (parity != 0) syndrome += bit_pos;
    }

    if (syndrome == 0) {
        printf("\n\nRESULT: No error detected.\n");
    } else {
        printf("\n\nRESULT: Error detected at bit position: %d", syndrome);
        
        
        code[syndrome] = (code[syndrome] == 0) ? 1 : 0;
        
        printf("\nCORRECTED DATA: ");
        for (int i = 1; i <= n; i++) printf("%d", code[i]);
        printf("\n");
    }

    close(new_sock);
    close(server_fd);
    return 0;
}