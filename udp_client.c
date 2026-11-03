#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 6541
#define SIZE 1024

int main(void)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    char message[SIZE], response[SIZE];
    for (;;) {
        printf("Enter command (UPPER/COUNT/REVERSE/EXIT): ");
        if (fgets(message, sizeof(message), stdin) == NULL) break;

        size_t len = strlen(message);
        if (len > 0 && message[len - 1] == '\n') message[--len] = '\0';

        if (sendto(sockfd, message, len, 0, (struct sockaddr *)&server_addr,
                   sizeof(server_addr)) < 0) {
            perror("sendto");
            break;
        }

        ssize_t n = recvfrom(sockfd, response, sizeof(response) - 1, 0, NULL, NULL);
        if (n < 0) {
            perror("recvfrom");
            break;
        }
        response[n] = '\0';
        printf("Server: %s\n", response);

        if (strcmp(message, "EXIT") == 0) break;
    }

    close(sockfd);
    return 0;
}
