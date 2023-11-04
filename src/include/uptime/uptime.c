#include <stdio.h>
#include <stdlib.h>
#include "uptime.h"

char *format_seconds(unsigned int seconds, size_t size)
{
    char *formatted_output = malloc((size+1)*sizeof(*formatted_output));
    if (seconds >= 60)
    {
        switch (seconds%60) // switch seconds part
        {
            case 0:
                sprintf(formatted_output, "%u minutes", seconds/60);
                break;
            case 1:
                sprintf(formatted_output, "%u minutes and 1 second", seconds/60);
                break;
            default:
                sprintf(formatted_output, "%u minutes and %u seconds", seconds/60, seconds%60);
        }
    }
    else
    {
        sprintf(formatted_output, "%u seconds", seconds);
    }
    return formatted_output;
}

char *get_uptime(size_t size)
{
    FILE *file = fopen("/proc/uptime", "r");
    if (!file)
    {
        fprintf(stderr, "error: '/proc/uptime' is inaccessible\n");
        return NULL;
    }

    char *uptime;
    unsigned int uptime_seconds = -1; // default value is integer limit, making errors obvious
    uptime = malloc((size+1)*sizeof(*uptime));

    fscanf(file, "%u", &uptime_seconds); // store value of file to an unsigned integer, flooring it
    fclose(file);

    uptime = format_seconds(uptime_seconds, size);

    return uptime;
}
