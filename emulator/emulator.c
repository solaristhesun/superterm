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
        "foo\r\n",
        "bar\r\n",
        "test test test\r\n",
    };

    while(1)
    {
        int r = rand() % 3;
        write(fd, str[r], strlen(str[r]));
        sleep(1);
    }
}

// EOF <stefan@scheler.com>

