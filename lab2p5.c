#include <stdio.h>

int main()
{
    unsigned int num, converted;
    unsigned char *ptr;
    int i;

    ptr = (unsigned char *)&num;

    if (*((unsigned char *)&(int){1}) == 1)
        printf("Host Machine: Little Endian\n");
    else
        printf("Host Machine: Big Endian\n");

    printf("Enter a number: ");
    scanf("%u", &num);

    ptr = (unsigned char *)&num;

    printf("Contents of each byte:\n");
    for (i = 0; i < 4; i++)
        printf("Byte %d = %u\n", i + 1, ptr[i]);

    converted = ((num & 0x000000FF) << 24) |
                ((num & 0x0000FF00) << 8) |
                ((num & 0x00FF0000) >> 8) |
                ((num & 0xFF000000) >> 24);

    printf("Converted Number = %u\n", converted);

    ptr = (unsigned char *)&converted;

    printf("Bytes after conversion:\n");
    for (i = 0; i < 4; i++)
        printf("Byte %d = %u\n", i + 1, ptr[i]);

    return 0;
}