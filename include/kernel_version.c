#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "kernel_version.h"

char *get_kernel_version(bool shorten, size_t size)
{
    FILE *file = fopen("/proc/version", "r");
    if (!file)
    {
        fprintf(stderr, "error: '/proc/version' (kernel information) is unreadable\n");
        return NULL;
    }

    char *kernel_version = malloc((size+1)*sizeof(*kernel_version));

    fgets(kernel_version, size, file);
    kernel_version[strlen(kernel_version) - 1] = 0; // trim the traling newline

    fclose(file);

    if (shorten)
    {
        char *kernel_version_short = malloc((size+1)*sizeof(*kernel_version_short));
        char *kernel_version_token = strtok(kernel_version, " ");

        // loop until the third word
        int i = 0;
        while (kernel_version_token != NULL && i < 3) 
        {
            // concatenate token formatted with a space after
            sprintf(kernel_version_short, "%s%s ", kernel_version_short, kernel_version_token);
            // next token
            kernel_version_token = strtok(NULL, " ");
            ++i;
        }

        free(kernel_version);

        return kernel_version_short;
    }

    return kernel_version;
}
