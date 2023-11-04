#ifndef ARGUMENT_PARSING_H_
#define ARGUMENT_PARSING_H_
#include <stdbool.h>
#include "../definitions/sizes.h"

struct Options
{
    bool use_ascii_file;
    bool use_order_file;
    bool accent_bold;
    bool shorten_kernel_version;
    char ascii_filename[FILENAME_SIZE];
    char order_filename[FILENAME_SIZE];
    char accent_color_char;
};

struct Options argument_parsing(int argc, char **argv, const char **argdefs_list[], unsigned int argdefs_list_size);

#endif
