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
    struct sockaddr_in myaddr;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
        printf("Failed\n");
    else
        printf("Socket created Successfully\n");

    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(6541);
    myaddr.sin_addr.s_addr = INADDR_ANY;
    int j = bind(sockfd, (struct sockaddr *)&myaddr, sizeof(myaddr));
    if (j == -1)
        printf("Failed\n");
    else
        printf("Bind Success\n");
    int s = sizeof(myaddr);
    int k = recvfrom(sockfd, msg, sizeof(msg), 0, (struct sockaddr *)&myaddr, &s);
    printf("%s\n", msg);
    printf("\nenter a message\n");
    gets(buf);
    int f = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&myaddr, sizeof(myaddr));

    close(sockfd);
    return 0;
}