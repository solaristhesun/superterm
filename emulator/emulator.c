#define _XOPEN_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main(void)
{
    int fd = open("/dev/ptmx", O_RDWR | O_NOCTTY);

    grantpt(fd);
    unlockpt(fd);

    printf("created virtual port %s\n", ptsname(fd));
    
    const char *str = "hallo\r\n";

    while(1)
    {
        write(fd, str, strlen(str));
        sleep(1);
    }
}

// EOF <stefan@scheler.com>

