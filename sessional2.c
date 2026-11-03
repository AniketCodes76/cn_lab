
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <server_ip> <port>\n", argv[0]);
        return 1;
    }
    int s, n;
    char msg[50], buf[50];
    struct sockaddr_in server;
    socklen_t len = sizeof(server);
    s = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    server.sin_addr.s_addr = inet_addr(argv[1]);
    while (1) {
        printf("Enter command (UPPER/REVERSE/COUNT/EXIT): ");
        fgets(msg, sizeof(msg), stdin);
        msg[strcspn(msg, "\n")] = '\0';
        sendto(s, msg, strlen(msg), 0,(struct sockaddr *)&server, len);
        memset(buf, 0, sizeof(buf));
        n = recvfrom(s, buf, sizeof(buf) - 1, 0, NULL, NULL);
        if (n < 0)
            continue;
        buf[n] = '\0';
        printf("Server response: %s\n", buf);
        if (strcmp(msg, "EXIT") == 0)
            break;
    }
    close(s);
    return 0;
}
