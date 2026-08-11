#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <Server IP> <Port>\n", argv[0]);
        return 1;
    }

    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    char *ip = argv[1];
    int port = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        perror("Socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connect");
        exit(1);
    }

    printf("Connected to Server.\n");

    while (1)
    {
        printf("\nClient: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        send(sockfd, buffer, strlen(buffer) + 1, 0);

        if (strcmp(buffer, "close") == 0)
            break;

        memset(buffer, 0, BUFFER_SIZE);
        recv(sockfd, buffer, BUFFER_SIZE, 0);

        printf("Server: %s\n", buffer);
    }

    close(sockfd);

    printf("Connection Closed.\n");

    return 0;
}