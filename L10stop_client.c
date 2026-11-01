
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);
    char packet[SIZE], ack[SIZE];
    int expected = 0;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind");
        close(sock);
        return 1;
    }

    printf("Receiver waiting for packets on port %d...\n", PORT);

    while (1) {
        int bytes = recvfrom(sock, packet, sizeof(packet), 0,
                             (struct sockaddr *)&client, &len);
        if (bytes < 0)
            continue;

        int seq;
        char *data = strchr(packet, '|');

        if (data == NULL || sscanf(packet, "%d", &seq) != 1)
            continue;

        *data = '\0';
        data++;

        if (seq == expected) {
            printf("Received packet %d: %s\n", seq, data);
            snprintf(ack, sizeof(ack), "ACK %d", seq);
            expected = 1 - expected;
        } else {
            printf("Duplicate packet %d discarded\n", seq);
            snprintf(ack, sizeof(ack), "ACK %d", 1 - expected);
        }

        sendto(sock, ack, strlen(ack) + 1, 0,
               (struct sockaddr *)&client, len);
        printf("Sent %s\n", ack);
    }

    close(sock);
    return 0;
}
