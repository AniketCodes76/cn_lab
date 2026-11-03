
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }
    int s;
    char msg[50], buf[50];
    int count = 0, n = 0;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);
    s = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));
    server.sin_addr.s_addr = INADDR_ANY;
    bind(s, (struct sockaddr *)&server, sizeof(server));
    while (1) {
        memset(msg, 0, sizeof(msg));
        memset(buf, 0, sizeof(buf));
        n = recvfrom(s, msg, sizeof(msg) - 1, 0,
                     (struct sockaddr *)&client, &len);
        if (n < 0)
            continue;
        msg[n] = '\0';
        printf("Original msg: %s\n", msg);
        if (strncmp(msg, "UPPER ", 6) == 0) {
            strcpy(buf, msg + 6);
            for (int i = 0; buf[i] != '\0'; i++)
                buf[i] = toupper((unsigned char)buf[i]);
        }
        else if (strncmp(msg, "REVERSE ", 8) == 0) {
            char *text = msg + 8;
            int l = strlen(text);
            for (int i = 0; i < l; i++)
                buf[i] = text[l - 1 - i];
            buf[l] = '\0';
        }
        else if (strncmp(msg, "COUNT ", 6) == 0) {
            char *text = msg + 6;
            count = 0;
            int in_word = 0;
            for (int i = 0; text[i] != '\0'; i++) {
                if (isspace((unsigned char)text[i]))
                    in_word = 0;
                else if (!in_word) {
                    count++;
                    in_word = 1;
                }
            }
            snprintf(buf, sizeof(buf), "%d", count);
        }
        else if (strcmp(msg, "EXIT") == 0) {
            strcpy(buf, "BYE");
            sendto(s, buf, strlen(buf), 0,
                   (struct sockaddr *)&client, len);
            break;
        }
        else {
            strcpy(buf, "Unknown command");
        }
        printf("Response: %s\n", buf);
        sendto(s, buf, strlen(buf), 0,
               (struct sockaddr *)&client, len);
    }
    close(s);
    return 0;
}
