#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <Receiver Port>\n", argv[0]);
        return 1;
    }

    int sockfd;
    int num, reverse = 0, remainder;

    struct sockaddr_in receiverAddr, senderAddr;
    socklen_t senderLen;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    memset(&receiverAddr, 0, sizeof(receiverAddr));

    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(atoi(argv[1]));
    receiverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd,
             (struct sockaddr *)&receiverAddr,
             sizeof(receiverAddr)) < 0)
    {
        perror("Bind failed");
        close(sockfd);
        return 1;
    }

    printf("Receiver waiting on port %s...\n", argv[1]);

    senderLen = sizeof(senderAddr);

    recvfrom(sockfd, &num, sizeof(num), 0,
             (struct sockaddr *)&senderAddr,
             &senderLen);

    printf("Received integer: %d\n", num);

    int temp = num;

    while (temp != 0)
    {
        remainder = temp % 10;
        reverse = reverse * 10 + remainder;
        temp = temp / 10;
    }

    printf("Reverse: %d\n", reverse);

    close(sockfd);

    return 0;
}