#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <Port>\n", argv[0]);
        return 1;
    }

    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    int port = atoi(argv[1]);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("Socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind");
        exit(1);
    }

    listen(server_fd, 5);
    printf("Server listening on port %d...\n", port);

    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0)
    {
        perror("Accept");
        exit(1);
    }

    printf("\nClient Connected!\n");
    printf("Client IP Address : %s\n", inet_ntoa(client_addr.sin_addr));
    printf("Client Port       : %d\n", ntohs(client_addr.sin_port));

    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        recv(client_fd, buffer, BUFFER_SIZE, 0);

        if (strcmp(buffer, "close") == 0)
        {
            printf("\nClient requested connection close.\n");
            break;
        }

        printf("\nClient: %s\n", buffer);

        printf("Server: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        send(client_fd, buffer, strlen(buffer) + 1, 0);
    }

    close(client_fd);
    close(server_fd);

    printf("Connection Closed.\n");

    return 0;
}