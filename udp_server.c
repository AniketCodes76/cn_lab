#include <arpa/inet.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
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
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(sockfd);
        return 1;
    }

    printf("UDP server listening on port %d...\n", PORT);
    for (;;) {
        char request[SIZE], response[SIZE];
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        ssize_t n = recvfrom(sockfd, request, sizeof(request) - 1, 0,
                             (struct sockaddr *)&client_addr, &client_len);
        if (n < 0) {
            perror("recvfrom");
            continue;
        }
        request[n] = '\0';

        char *command = strtok(request, " \t\r\n");
        char *text = strtok(NULL, "\r\n");
        if (text != NULL)
            while (*text == ' ' || *text == '\t') text++;

        int should_exit = 0;
        if (command == NULL) {
            strcpy(response, "Please send a command");
        } else if (strcmp(command, "UPPER") == 0) {
            if (text == NULL) text = "";
            size_t i;
            for (i = 0; text[i] != '\0' && i < sizeof(response) - 1; i++)
                response[i] = (char)toupper((unsigned char)text[i]);
            response[i] = '\0';
        } else if (strcmp(command, "REVERSE") == 0) {
            if (text == NULL) text = "";
            size_t len = strlen(text);
            if (len >= sizeof(response)) len = sizeof(response) - 1;
            for (size_t i = 0; i < len; i++) response[i] = text[len - 1 - i];
            response[len] = '\0';
        } else if (strcmp(command, "COUNT") == 0) {
            int words = 0, in_word = 0;
            if (text != NULL) {
                for (size_t i = 0; text[i] != '\0'; i++) {
                    if (isspace((unsigned char)text[i])) in_word = 0;
                    else if (!in_word) { words++; in_word = 1; }
                }
            }
            snprintf(response, sizeof(response), "%d", words);
        } else if (strcmp(command, "EXIT") == 0) {
            strcpy(response, "BYE");
            should_exit = 1;
        } else {
            strcpy(response, "Unknown command");
        }

        sendto(sockfd, response, strlen(response), 0,
               (struct sockaddr *)&client_addr, client_len);
        if (should_exit) break;
    }

    close(sockfd);
    return 0;
}
