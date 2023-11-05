#ifndef ARGUMENT_PARSING_H_
#define ARGUMENT_PARSING_H_
#include <stdbool.h>
#include "../definitions/sizes.h"

typedef struct
{
    bool help_mode;
    bool use_ascii_file;
    bool bold;
    bool kernel_shorten;
    bool use_order_file;

    char color_char;
    char ascii_file[FILENAME_SIZE];
    char order_file[FILENAME_SIZE];
}Options;

Options argument_parsing(int argc, char **argv);

#endif
