#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "argument_parsing.h"

bool is_arg(const char *argdef[], const char *string)
{
    return strcmp(string, argdef[0]) == 0 || strcmp(string, argdef[1]) == 0;
}

void help_menu_print(const char **argdefs_list[], unsigned int argdefs_list_size)
{
    printf("usage: lowfetch [options]\n");
    for(int i = 0; i < argdefs_list_size; ++i)
    {
        printf("%s, %s,\t%s\n", argdefs_list[i][0], argdefs_list[i][1], argdefs_list[i][2]);
    }
}

struct Options argument_parsing(int argc, char **argv, const char **argdefs_list[], unsigned int argdefs_list_size)
{
    struct Options option = {false, false, false, false, "", "", 'w'};
    if (argc > 1)
    {
        for (int i = 1; i < argc; ++i)
        {
            // -h, --help
            if (is_arg(argdefs_list[0], argv[i]))
            {
                help_menu_print(argdefs_list, argdefs_list_size);
                exit(0);
            }
            // -a, --ascii
            if (is_arg(argdefs_list[1], argv[i]))
            {
                option.use_ascii_file = true;
                if (argv[i+1] && argv[i+1][0] != '-')
                {
                    strcpy(option.ascii_filename, argv[i+1]);
                }
                continue;
            }
            // -o, --order
            if (is_arg(argdefs_list[2], argv[i]))
            {
                option.use_order_file = true;
                if (argv[i+1] && argv[i+1][0] != '-')
                {
                    strcpy(option.order_filename, argv[i+1]);
                }
                continue;
            }
            // -c, --color
            if (is_arg(argdefs_list[3], argv[i]))
            {
                if (argv[i+1] && argv[i+1][0] != '-')
                {
                    option.accent_color_char = argv[i+1][0];
                }
                continue;
            }
            // -b, --bold
            if (is_arg(argdefs_list[4], argv[i]))
            {
                option.accent_bold = true;
                continue;
            }
            // -ks, --kernel-shorten
            if (is_arg(argdefs_list[5], argv[i]))
            {
                option.shorten_kernel_version = true;
                continue;
            }
        }
    }
    return option;
}
