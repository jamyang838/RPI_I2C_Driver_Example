#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

int main( void )
{
    char *buf = "test 123\n";
    void *buf_read;    
    int fd;
    int len;
    int len_read;
    fd = open("/dev/i2c-1", O_RDWR);
    if (ioctl(fd,I2C_SLAVE,0x08) < 0)
    {
        printf("I2C fail");
    }    
    printf("fd = %d\n", fd);
    len = write(fd, buf, strlen(buf));  
    printf("legnth of transmittion = %d\n", len);    
    close(fd);
    return 0;
}