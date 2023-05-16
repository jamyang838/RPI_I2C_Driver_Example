#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main( void )
{
    char *buf = "asdasdasd";
    open();
    write(3, buf, 5);
    printf("asdasd");
    return 0;
}