#include <stdio.h>

int main()
{
    unsigned int num, converted;
    unsigned char *ptr;
    int i;

    unsigned int x = 1;
    char *c = (char *)&x;

    if (*c == 1)
        printf("Host Machine is Little Endian\n");
    else
        printf("Host Machine is Big Endian\n");

    printf("Enter an unsigned integer: ");
    scanf("%u", &num);

    printf("\nOriginal Number = %u\n", num);

    ptr = (unsigned char *)&num;

    printf("Byte contents of Original Number:\n");
    for (i = 0; i < sizeof(num); i++)
        printf("Byte %d : 0x%02X\n", i, ptr[i]);

    converted = ((num & 0x000000FF) << 24) |
                ((num & 0x0000FF00) << 8)  |
                ((num & 0x00FF0000) >> 8)  |
                ((num & 0xFF000000) >> 24);

    printf("\nConverted Number = %u\n", converted);

    ptr = (unsigned char *)&converted;

    printf("Byte contents of Converted Number:\n");
    for (i = 0; i < sizeof(converted); i++)
        printf("Byte %d : 0x%02X\n", i, ptr[i]);

    return 0;
}