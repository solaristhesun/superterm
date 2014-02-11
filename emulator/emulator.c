#define _XOPEN_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

int main(void)
{
    srand(time(NULL));

    int fd = open("/dev/ptmx", O_RDWR | O_NOCTTY);

    grantpt(fd);
    unlockpt(fd);

    printf("created virtual port %s\n", ptsname(fd));
    
    const char *str[] = {
        "foo",
        "bar",
        "test test test",
        "\teins\tzwei\tdrei\tvier\tfuenf",
        "01234567890123456789012345678901234567890123456789012345678901234567890123456789"
    };

    unsigned long c = 0;

    while(1)
    {
        int r = rand() % 5;
        
        char msg[100];
        sprintf(msg, "%08lx: %s\r\n", c++, str[r]);

        write(fd, msg, strlen(msg));
        usleep(100000);
    }
}

// EOF <stefan@scheler.com>

