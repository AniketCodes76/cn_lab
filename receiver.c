#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define SIZE 1024
int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("Usage: %s <My Port> <Sender IP> <Sender Port> <Name>\n", argv[0]);
        return 1;
    }
    int sockfd;
    struct sockaddr_in myAddr, otherAddr;
    socklen_t len = sizeof(otherAddr);
    char sendMsg[SIZE], recvMsg[SIZE];
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    myAddr.sin_family = AF_INET;
    myAddr.sin_addr.s_addr = INADDR_ANY;
    myAddr.sin_port = htons(atoi(argv[1]));
    bind(sockfd, (struct sockaddr *)&myAddr, sizeof(myAddr));
    otherAddr.sin_family = AF_INET;
    otherAddr.sin_port = htons(atoi(argv[3]));
    inet_pton(AF_INET, argv[2], &otherAddr.sin_addr);
    printf("%s started...\n", argv[4]);
    while (1)
    {
        recvfrom(sockfd, recvMsg, SIZE, 0,
                 (struct sockaddr *)&otherAddr, &len);

        printf("Peer: %s\n", recvMsg);

        if(strcmp(recvMsg, "exit") == 0)
            break;

        printf("You: ");
        fgets(sendMsg, SIZE, stdin);
        sendMsg[strcspn(sendMsg, "\n")] = '\0';

        sendto(sockfd, sendMsg, strlen(sendMsg)+1, 0,
               (struct sockaddr *)&otherAddr, len);

        if(strcmp(sendMsg, "exit") == 0)
            break;
    }
    close(sockfd);
    return 0;
}