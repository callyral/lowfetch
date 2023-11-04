#include <stdbool.h>
#include "colors.h"

char *get_ansi_color_from(char color_char, bool bold)
{
    if (!bold)
    {
        switch (color_char)
        {
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
    return ANSI_COLOR_RESET;
}
