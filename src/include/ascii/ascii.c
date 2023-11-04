#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "ascii.h"
#include "../general/file_read.h"

char *get_ascii(bool use_file, const char *filename, size_t size)
{
    char *ascii;

    if (!use_file)
    {
        ascii = malloc((size+1) * sizeof(*ascii));
        strcpy(ascii, ascii_default);
        return ascii;
    }

    if (access(filename, R_OK) == -1)
    {
        fprintf(stderr, "error: '%s' is unreadable or doesn't exist\n", filename);

        ascii = malloc((size+1) * sizeof(*ascii));
        strcpy(ascii, ascii_default);
        return ascii;
    }
    else
    {
        ascii = file_read(filename, size);
        return ascii;
    }
}
