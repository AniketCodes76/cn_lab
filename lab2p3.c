#include <stdio.h>

int main()
{
    unsigned int num;
    unsigned char ch1, ch2, ch3, ch4;

    printf("Enter a number: ");
    scanf("%u", &num);

    ch1 = num & 0xFF;
    ch2 = (num >> 8) & 0xFF;
    ch3 = (num >> 16) & 0xFF;
    ch4 = (num >> 24) & 0xFF;

    printf("Byte 1 = %u\n", ch1);
    printf("Byte 2 = %u\n", ch2);
    printf("Byte 3 = %u\n", ch3);
    printf("Byte 4 = %u\n", ch4);

    return 0;
}