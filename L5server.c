#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_fd)
{
    char buffer[BUFFER_SIZE];
    int n;

    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);

        n = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);

        if (n <= 0)
            break;

        buffer[n] = '\0';

        printf("Client: %s", buffer);

        if (strncmp(buffer, "logout", 6) == 0)
        {
            printf("Client logged out.\n");
            break;
        }

        printf("Server: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        send(client_fd, buffer, strlen(buffer), 0);
    }

    close(client_fd);
    exit(0);
}

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    pid_t pid;

    signal(SIGCHLD, SIG_IGN);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        close(server_fd);
        exit(1);
    }

    if (listen(server_fd, 10) < 0)
    {
        perror("Listen failed");
        close(server_fd);
        exit(1);
    }

    printf("Chat server started on port %d...\n", PORT);

    while (1)
    {
        client_len = sizeof(client_addr);

        client_fd = accept(server_fd,
                           (struct sockaddr *)&client_addr,
                           &client_len);

        if (client_fd < 0)
        {
            perror("Accept failed");
            continue;
        }

        printf("Client connected.\n");

        pid = fork();

        if (pid < 0)
        {
            perror("Fork failed");
            close(client_fd);
        }
        else if (pid == 0)
        {
            close(server_fd);
            handle_client(client_fd);
        }
        else
        {
            close(client_fd);
        }
    }

    close(server_fd);
    return 0;
}