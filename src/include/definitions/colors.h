#ifndef COLORS_H_
#define COLORS_H_
#include <stdbool.h>

// color chars
#define CHAR_WHITE   'w'
#define CHAR_RED     'r'
#define CHAR_GREEN   'g'
#define CHAR_YELLOW  'y'
#define CHAR_BLUE    'b'
#define CHAR_MAGENTA 'm'
#define CHAR_CYAN    'c'

// color codes
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define ANSI_COLOR_RED_BOLD     "\x1b[31;1m"
#define ANSI_COLOR_GREEN_BOLD   "\x1b[32;1m"
#define ANSI_COLOR_YELLOW_BOLD  "\x1b[33;1m"
#define ANSI_COLOR_BLUE_BOLD    "\x1b[34;1m"
#define ANSI_COLOR_MAGENTA_BOLD "\x1b[35;1m"
#define ANSI_COLOR_CYAN_BOLD    "\x1b[36;1m"
#define ANSI_COLOR_RESET_BOLD   "\x1b[0;1m"

char *get_ansi_color_from(char color_char, bool bold);

#endif
