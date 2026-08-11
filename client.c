#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main()
{
    char msg[500], buf[500];
    struct sockaddr_in serveraddr;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
        printf("Failed\n");
    else
        printf("Socket created Successfully\n");

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(6541);
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    int j = bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (j == -1)
        printf("Failed\n");
    else
        printf("Bind Success\n");
    printf("enter message");
    gets(msg);
    int z = sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    int s = sizeof(serveraddr);
    int k = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&serveraddr, &s);
    printf("\nMessage from server\n");
    printf("%s\n", buf);

    close(sockfd);
    return 0;
}