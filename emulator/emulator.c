#define _XOPEN_SOURCE 501
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    srand(time(NULL));

    int fd = open("/dev/tnt0", O_RDWR | O_NOCTTY);

    if (argc < 2)
    {
        return 1;
    }

   // printf("created virtual port %s\n", ptsname(fd));
    
    unsigned long c = 0;

    FILE* fp = fopen(argv[1], "r");

	if (fp != NULL)
	{
		printf("starting playback of %s...\n", argv[1]);    
		
		while(1)
		{
			
			char msg[100];

			fgets(&msg, 100, fp);

			if (fgets(&msg, 100, fp) != 0)
			{
				printf("{%s}\n", msg);
				write(fd, msg, strlen(msg));
			}
			else
			{
				fseek(fp, 0, SEEK_SET);
			}

			usleep(50000);
		}

		fclose(fp);
	}
}

// EOF <stefan@scheler.com>

