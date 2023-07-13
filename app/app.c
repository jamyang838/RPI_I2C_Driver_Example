/**********************************
 * Demo I2C Transmiting and Reciving
 * this app will send a command to slave and then read 25 bytes from slave.
 * 1. change WRITE_COMMAND to the command that you want to send to slave.
 * 2. compile
 * 3. run it. Perhapes needs run as admin.
**********************************/

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define WRITE_COMMAND   "$510;\r"

int main( void )
{
    char buf[strlen(WRITE_COMMAND)];
    memset(buf,0,strlen(WRITE_COMMAND));
    strcpy(buf,WRITE_COMMAND);
    int fd;
    int len;
    void *buf_read;
    int len_read;

    fd = open("/dev/arduino_i2c", O_RDWR);
    printf("*********************Handller************************\n");
    printf("fd = %d\n", fd);
    len = write(fd, buf, strlen(buf));
    printf("*********************Transmit************************\n");
    printf("ASCII = %s\n", buf);
    buf_read = malloc(25);
    len_read = read(fd, buf_read, 25);
    printf("*********************Recive**************************\n");
    printf("Bytes: \t");
    for (size_t i = 0; i < 25; i++)
    {        
        printf("%x ", *(char*)(buf_read+i));
    }    
    printf("\n");
    printf("ASCII: \t%s\n", (char*)buf_read);
    return 0;
}