
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

#define PORT 8080
#define SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server;
    socklen_t len = sizeof(server);
    char msg[SIZE], ack[SIZE];
    int seq = 0, n;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    struct timeval timeout = {3, 0};
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO,
               &timeout, sizeof(timeout));

    printf("Enter number of packets: ");
    scanf("%d", &n);
    getchar();

    for (int i = 0; i < n; i++) {
        printf("Enter packet %d data: ", i + 1);
        fgets(msg, SIZE - 1, stdin);
        msg[strcspn(msg, "\n")] = '\0';

        int received = 0;
        while (!received) {
            char packet[SIZE];
            snprintf(packet, sizeof(packet), "%d|%s", seq, msg);

            sendto(sock, packet, strlen(packet) + 1, 0,
                   (struct sockaddr *)&server, len);
            printf("Sent packet with sequence number %d\n", seq);

            int bytes = recvfrom(sock, ack, sizeof(ack), 0,
                                 (struct sockaddr *)&server, &len);

            if (bytes > 0) {
                int ackno = -1;
                if (sscanf(ack, "ACK %d", &ackno) == 1
                    && ackno == seq) {
                    printf("Received ACK %d\n", ackno);
                    received = 1;
                } else {
                    printf("Unexpected ACK. Waiting for correct ACK.\n");
                }
            } else {
                printf("Timeout! Retransmitting packet %d\n", seq);
            }
        }

        seq = 1 - seq;
    }

    printf("All packets sent successfully.\n");
    close(sock);
    return 0;
}
