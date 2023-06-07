#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( void )
{
    char *buf = "test\n";    
    int fd;
    int len;

    void *buf_read;
    int len_read;

    fd = open("/dev/arduino_i2c", O_RDWR);
    printf("fd = %d\n", fd);
    len = write(fd, buf, strlen(buf));    
    buf_read = malloc(10);
    len_read = read(fd, buf_read, 10);
    printf("legnth of transmittion = %s\n", (char*)buf_read);
    return 0;
}