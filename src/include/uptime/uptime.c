#include <stdio.h>
#include <stdlib.h>
#include "../definitions/sizes.h"
#include "uptime.h"

char *format_seconds(unsigned int seconds, size_t size)
{
    unsigned int seconds_part = seconds%60;
    unsigned int minutes_part = seconds/60;
    char *formatted_output = malloc((size+1)*sizeof(*formatted_output));
    if (minutes_part < 1)
    {
        // 1 second is not handled since it's very unlikely.
        sprintf(formatted_output, "%u seconds", seconds);
        return formatted_output;
    }

    // 1 minute (singular)
    if (minutes_part == 1)
    {
        switch (seconds_part)
        {
            case 0:
                sprintf(formatted_output, "%u minute", minutes_part);
                break;
            case 1:
                sprintf(formatted_output, "%u minute and 1 second", minutes_part);
                break;
            default:
                sprintf(formatted_output, "%u minute and %u seconds", minutes_part, seconds_part);
        }
        return formatted_output;
    }

    // More than 1 minute or 0 minutes (plural)
    switch (seconds_part)
    {
        case 0:
            sprintf(formatted_output, "%u minutes", minutes_part);
            break;
        case 1:
            sprintf(formatted_output, "%u minutes and 1 second", minutes_part);
            break;
        default:
            sprintf(formatted_output, "%u minutes and %u seconds", minutes_part, seconds_part);
    }

    return formatted_output;
}

char *get_uptime()
{
    FILE *file = fopen("/proc/uptime", "r");
    if (!file)
    {
        fprintf(stderr, "error: '/proc/uptime' is inaccessible\n");
        return NULL;
    }

    char *uptime;
    unsigned int uptime_seconds = -1; // default value is integer limit, making errors obvious
    uptime = malloc((UPTIME_SIZE+1)*sizeof(*uptime));

    fscanf(file, "%u", &uptime_seconds); // store value of file to an unsigned integer, flooring it
    fclose(file);

    uptime = format_seconds(uptime_seconds, UPTIME_SIZE);

    return uptime;
}
