
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <receiver_ip> <receiver_port> <sender_port>\n", argv[0]);
        return 1;
    }

    int sockfd;
    struct sockaddr_in sender, receiver;
    socklen_t len = sizeof(receiver);
    char msg[1024];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    memset(&sender, 0, sizeof(sender));
    sender.sin_family = AF_INET;
    sender.sin_addr.s_addr = INADDR_ANY;
    sender.sin_port = htons(atoi(argv[3]));

    if (bind(sockfd, (struct sockaddr *)&sender, sizeof(sender)) < 0) {
        perror("bind");
        close(sockfd);
        return 1;
    }

    memset(&receiver, 0, sizeof(receiver));
    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(atoi(argv[2]));

    if (inet_pton(AF_INET, argv[1], &receiver.sin_addr) != 1) {
        printf("Invalid IP address\n");
        close(sockfd);
        return 1;
    }

    printf("Sender started. Type messages (exit to quit).\n");

    while (1) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        FD_SET(sockfd, &readfds);

        int maxfd = sockfd > STDIN_FILENO ? sockfd : STDIN_FILENO;

        if (select(maxfd + 1, &readfds, NULL, NULL, NULL) < 0) {
            perror("select");
            break;
        }

        if (FD_ISSET(sockfd, &readfds)) {
            int n = recvfrom(sockfd, msg, sizeof(msg) - 1, 0,
                             (struct sockaddr *)&receiver, &len);
            if (n < 0) {
                perror("recvfrom");
                break;
            }

            msg[n] = '\0';
            printf("\nReceiver: %s\n", msg);

            if (strcmp(msg, "exit") == 0) {
                printf("Receiver requested termination.\n");
                break;
            }
            printf("You: ");
            fflush(stdout);
        }

        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            if (fgets(msg, sizeof(msg), stdin) == NULL)
                break;

            msg[strcspn(msg, "\n")] = '\0';

            if (sendto(sockfd, msg, strlen(msg), 0,
                       (struct sockaddr *)&receiver, sizeof(receiver)) < 0) {
                perror("sendto");
                break;
            }

            if (strcmp(msg, "exit") == 0) {
                printf("Closing sender.\n");
                break;
            }
        }
    }

    close(sockfd);
    return 0;
}
