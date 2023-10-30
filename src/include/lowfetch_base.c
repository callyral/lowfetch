#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "color_definitions.h"
#include "lowfetch_base.h"

char *file_read(const char *filename, size_t size)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "error: '%s' is inaccessible or does not exist\n", filename);
        return NULL;
    }

    char *output;
    output = malloc((size+1)*sizeof(*output)); 

    fread(output, size, 1, file); // store the file into output
    output[strlen(output) - 1] = 0; // trim the trailing newline

    fclose(file); // close the file since it's already been stored

    return output;
}

char *get_ansi_color_from(char color_char, bool bold)
{
    if (!bold)
    {
        switch (color_char)
        {
            case CHAR_WHITE:
                return ANSI_COLOR_RESET;
            case CHAR_RED:
                return ANSI_COLOR_RED;
            case CHAR_GREEN:
                return ANSI_COLOR_GREEN;
            case CHAR_YELLOW:
                return ANSI_COLOR_YELLOW;
            case CHAR_BLUE:
                return ANSI_COLOR_BLUE;
            case CHAR_MAGENTA:
                return ANSI_COLOR_MAGENTA;
            case CHAR_CYAN:
                return ANSI_COLOR_CYAN;
            default:
                return ANSI_COLOR_RESET;
        }
    }
    else
    {
        switch (color_char)
        {
            case CHAR_WHITE:
                return ANSI_COLOR_RESET_BOLD;
            case CHAR_RED:
                return ANSI_COLOR_RED_BOLD;
            case CHAR_GREEN:
                return ANSI_COLOR_GREEN_BOLD;
            case CHAR_YELLOW:
                return ANSI_COLOR_YELLOW_BOLD;
            case CHAR_BLUE:
                return ANSI_COLOR_BLUE_BOLD;
            case CHAR_MAGENTA:
                return ANSI_COLOR_MAGENTA_BOLD;
            case CHAR_CYAN:
                return ANSI_COLOR_CYAN_BOLD;
            default:
                return ANSI_COLOR_RESET_BOLD;
        }
    }
}

