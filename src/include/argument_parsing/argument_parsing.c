#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "argument_parsing.h"
#include "../definitions/colors.h"
#include "../definitions/sizes.h"

// argument definition
typedef struct
{
    char key_short;
    char *key_long;
    char *description;
}ArgumentDef;

#define ARGUMENTLIST_ARRAY_SIZE 6
typedef union
{
    struct {
        ArgumentDef help;
        ArgumentDef ascii_file;
        ArgumentDef bold;
        ArgumentDef color;
        ArgumentDef kernel_shorten;
        ArgumentDef order_file;
    };
    ArgumentDef array[ARGUMENTLIST_ARRAY_SIZE];
}ArgumentList;

static ArgumentList argument_list = {
        .help =           {'h', "--help",           "show this help menu"},
        .ascii_file =     {'a', "--ascii",          "select ascii file"},
        .bold =           {'b', "--bold",           "enable boldness"},
        .color =          {'c', "--color",          "select color"},
        .kernel_shorten = {'k', "--kernel-shorten", "enable shorter kernel version"},
        .order_file =     {'o', "--order",          "select order file"}
};

bool is_argument(char *string)
{
    if (!string)
    {
        return false;
    }
    return string[0] == '-';
}

bool is_short_argument(char *string)
{
    // checks if string starts with only one dash
    return is_argument(string) && !(string[1] == '-');
}

bool compare_to_argument_def(char *input_key, ArgumentDef def)
{
    if (!is_short_argument(input_key))
    {
        return strcmp(input_key, def.key_long) == 0;
    }
    //  since it's a short argument, start looping from first letter
    // at index 1
    for (int i = 1; i < strlen(input_key); ++i)
    {
        if (input_key[i] == def.key_short)
        {
            return true;
        }
    }
    return false;
}

void print_help_menu()
{
    printf("usage: lowfetch [options]\n");
    for(int i = 0; i < ARGUMENTLIST_ARRAY_SIZE; ++i)
    {
        printf("%c%c, %s,\t%s\n", '-', argument_list.array[i].key_short, argument_list.array[i].key_long, argument_list.array[i].description);
    }
}
#undef ARGUMENTLIST_ARRAY_SIZE

Options argument_parsing(int argc, char **argv)
{
    Options options = {
            .help_mode = false,
            .use_ascii_file = false,
            .bold = false,
            .kernel_shorten = false,
            .use_order_file = false,
            .color_char = CHAR_WHITE,
    };

    if (getenv("XDG_CONFIG_HOME"))
    {
        strcpy(options.ascii_file, getenv("XDG_CONFIG_HOME"));
        strcat(options.ascii_file, "/lowfetch/ascii");

        strcpy(options.order_file, getenv("XDG_CONFIG_HOME"));
        strcat(options.order_file, "/lowfetch/order"); 
    }
    else
    {
        strcpy(options.ascii_file, getenv("HOME"));
        strcat(options.ascii_file, "/.config/lowfetch/ascii"); 

        strcpy(options.order_file, getenv("HOME"));
        strcat(options.order_file, "/.config/lowfetch/order"); 
    }

    if (argc <= 1) { return options; }

    for (int i = 1; i < argc; ++i)
    {
        options.help_mode =      compare_to_argument_def(argv[i], argument_list.help) || options.help_mode;
        options.bold =           compare_to_argument_def(argv[i], argument_list.bold) || options.bold;
        options.kernel_shorten = compare_to_argument_def(argv[i], argument_list.kernel_shorten) || options.kernel_shorten;

        if (compare_to_argument_def(argv[i], argument_list.ascii_file))
        {
            options.use_ascii_file = true;
            if (!is_argument(argv[i+1]))
            {
                strcpy(options.ascii_file, argv[i+1]);
            }
        }
        if (compare_to_argument_def(argv[i], argument_list.order_file))
        {
            options.use_order_file = true;
            if (!is_argument(argv[i+1]))
            {
                strcpy(options.order_file, argv[i+1]);
            }
        }
        if (compare_to_argument_def(argv[i], argument_list.color) && !is_argument(argv[i+1]) )
        {
            options.color_char = argv[i+1][0];
        }
    }

    if (options.help_mode) { print_help_menu(); exit(EXIT_SUCCESS); }

    return options;
}
