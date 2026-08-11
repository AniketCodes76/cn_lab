#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd,
                (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        close(sockfd);
        exit(1);
    }

    printf("Connected to chat server.\n");

    while (1)
    {
        printf("You: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        send(sockfd, buffer, strlen(buffer), 0);

        if (strncmp(buffer, "logout", 6) == 0)
        {
            printf("Logged out.\n");
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);

        int n = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);

        if (n <= 0)
            break;

        buffer[n] = '\0';

        printf("Server: %s", buffer);
    }

    close(sockfd);
    return 0;
}