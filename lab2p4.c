#include <stdio.h>

struct pkt
{
    char ch1;
    char ch2[2];
    char ch3;
};

int main()
{
    unsigned int num, original;
    struct pkt p;

    printf("Enter a number: ");
    scanf("%u", &num);

    p.ch1 = num & 0xFF;
    p.ch2[0] = (num >> 8) & 0xFF;
    p.ch2[1] = (num >> 16) & 0xFF;
    p.ch3 = (num >> 24) & 0xFF;

    printf("ch1 = %u\n", (unsigned char)p.ch1);
    printf("ch2[0] = %u\n", (unsigned char)p.ch2[0]);
    printf("ch2[1] = %u\n", (unsigned char)p.ch2[1]);
    printf("ch3 = %u\n", (unsigned char)p.ch3);

    original = ((unsigned int)(unsigned char)p.ch3 << 24) |
               ((unsigned int)(unsigned char)p.ch2[1] << 16) |
               ((unsigned int)(unsigned char)p.ch2[0] << 8) |
               (unsigned int)(unsigned char)p.ch1;

    printf("Original number = %u\n", original);

    return 0;
}