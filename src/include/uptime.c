#include <stdio.h>
#include <stdlib.h>
#include "uptime.h"

char *get_uptime(size_t size)
{
    FILE *file = fopen("/proc/uptime", "r");
    if (!file)
    {
        fprintf(stderr, "error: uptime is inaccessible\n");
        return NULL;
    }

    char *uptime;
    unsigned int uptime_seconds = -1; // default value is integer limit, making errors obvious
    uptime = malloc((size+1)*sizeof(*uptime));

    fscanf(file, "%u", &uptime_seconds); // store value of file to an unsigned integer, flooring (rounding) it
    // format uptime string accordingly
    if (uptime_seconds >= 60)
    {
        switch (uptime_seconds%60) // switch seconds
        {
            case 0:
                sprintf(uptime, "%u minutes", uptime_seconds/60);
                break;
            case 1:
                sprintf(uptime, "%u minutes and 1 second", uptime_seconds/60);
                break;
            default:
                sprintf(uptime, "%u minutes and %u seconds", uptime_seconds/60, uptime_seconds%60);
        }
    }
    else
    {
        sprintf(uptime, "%u seconds", uptime_seconds);
    }

    fclose(file);

    return uptime;
}
