#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <Receiver IP> <Receiver Port>\n", argv[0]);
        return 1;
    }

    int sockfd;
    int num;

    struct sockaddr_in receiverAddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    memset(&receiverAddr, 0, sizeof(receiverAddr));

    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(atoi(argv[2]));
    receiverAddr.sin_addr.s_addr = inet_addr(argv[1]);

    printf("Enter an integer: ");
    scanf("%d", &num);

    sendto(sockfd, &num, sizeof(num), 0,
           (struct sockaddr *)&receiverAddr,
           sizeof(receiverAddr));

    printf("Integer sent: %d\n", num);

    close(sockfd);

    return 0;
}