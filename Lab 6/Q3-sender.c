#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char data[10];
    int code[20], m, r = 0;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8082);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed\n");
        return -1;
    }

    printf("Enter binary data (e.g., 1011): ");
    scanf("%s", data);
    m = strlen(data);

    while ((1 << r) < (m + r + 1)) r++;

    int n = m + r;
    int j = 0;

    
    for (int i = 1; i <= n; i++) {
        if ((i & (i - 1)) == 0) { 
            code[i] = 0; 
        } else {
            code[i] = data[j++] - '0';
        }
    }

    
    for (int i = 0; i < r; i++) {
        int pos = (1 << i); 
        int parity = 0;
        for (int l = 1; l <= n; l++) {
            if (((l >> i) & 1) && (l != pos)) {
                parity ^= code[l];
            }
        }
        code[pos] = parity;
    }

    printf("Encoded Hamming Code to send: ");
    for (int i = 1; i <= n; i++) printf("%d", code[i]);
    printf("\n");

    
    printf("\n\nDo you want to inject an error before sending? (1-Yes, 0-No): ");
    int inject;
    scanf("%d", &inject);
    
    if (inject == 1) {
        int bit_pos;
        printf("Enter bit position to flip (1 to %d): ", n);
        scanf("%d", &bit_pos);
        if (bit_pos >= 1 && bit_pos <= n) {
            code[bit_pos] = !code[bit_pos]; 
            printf("Data corrupted! New string to be sent: ");
            for (int i = 1; i <= n; i++) printf("%d", code[i]);
            printf("\n");
        }
    }

    send(sock, code, sizeof(code), 0);
    printf("Data sent successfully.\n");

    close(sock);
    return 0;
}