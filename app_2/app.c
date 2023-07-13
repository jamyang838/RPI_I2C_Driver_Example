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
    // char buf[10] = "$510;\r";
    char buf[10];
    strcpy(buf, "$510;\r");      
    // char *buf = "$510;\r";
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
    len = write(fd, buf, 10);  
    printf("legnth of transmittion = %d\n", len);    
    close(fd);
    return 0;
}