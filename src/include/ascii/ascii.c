#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include "../general/file_read.h"
#include "../definitions/sizes.h"
#include "ascii.h"


char *get_ascii(bool use_file, const char *filename)
{
    char *ascii;

    if (!use_file)
    {
        ascii = malloc((ASCII_FILESIZE+1) * sizeof(*ascii));
        strcpy(ascii, ascii_default);
        return ascii;
    }

    if (access(filename, R_OK) == -1)
    {
        fprintf(stderr, "error: '%s' (ascii) is unreadable or doesn't exist\n", filename);

        ascii = malloc((ASCII_FILESIZE+1) * sizeof(*ascii));
        strcpy(ascii, ascii_default);
        return ascii;
    }
    else
    {
        ascii = file_read(filename, ASCII_FILESIZE);
        return ascii;
    }
}
