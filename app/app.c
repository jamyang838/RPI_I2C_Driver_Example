#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main( void )
{
    char *buf = "test\n";
    int fd;
    int len;
    fd = open("/dev/arduino_i2c", O_RDWR);
    printf("fd = %d\n", fd);
    len = write(fd, buf, strlen(buf));
    printf("legnth of transmittion = %d\n", len);
    return 0;
}