#include "lowfetch.h"
#include "include/colors.h"
#include <string.h>
#define ASCII_FILE_SIZE 4096
#define DEFAULT_SIZE 256

/* from https://www.asciiart.eu/animals/cats */
static char *ascii_default = " |\\'/-..--.\n"
                             " / _ _   ,  ;\n"
                             "`~=`Y'~_<._./\n"
                             " <`-....__.'";

enum SystemChars
{
    CHAR_ASCII = 'a',
    CHAR_DISTRO_ID = 'd',
    CHAR_KERNEL_VERSION = 'k',
    CHAR_UPTIME = 'u'
};

struct SystemInfo
{
    char *ascii;
    char *distro_id;
    char *kernel_version;
    char *uptime;
};

int main(int argc, char **argv)
{
    enum ColorChars accent_color_char = CHAR_WHITE; // white means no color
    bool use_ascii_file = false;
    bool accent_bold = false;
    
    char *ascii_filename;
    /* use XDG_CONFIG_HOME if it exists, otherwise default to ~/.config/ */
    if (getenv("XDG_CONFIG_HOME"))
    {
        ascii_filename = strcat(getenv("XDG_CONFIG_HOME"), "/lowfetch/ascii"); 
    }
    else
    {
        ascii_filename = strcat(getenv("HOME"), "/.config/lowfetch/ascii"); 
    }
    
    /* CLI argument parsing */
    /* format: { short, long, desc }*/
    const char *argdef_help[] = {"-h", "--help", "show this help menu"};
    const char *argdef_ascii_file[] = {"-a", "--ascii", "select ascii file"};
    const char *argdef_color[] = {"-c", "--color", "select color"};
    const char *argdef_bold[] = {"-b", "--bold", "toggle bold colors"};
    // arguments to include in help menu
    #define ARGDEFS_LIST_SIZE 4
    const char **argdefs_list[ARGDEFS_LIST_SIZE] = {argdef_help, argdef_ascii_file, argdef_color, argdef_bold};
    if (argc > 1)
    {
        for (int i = 1; i < argc; ++i)
        {
            if (arg_parse(argdef_ascii_file, argv[i]))
            {
                use_ascii_file = true;
                if (argv[i+1] && argv[i+1][0] != '-')
                {
                    ascii_filename = argv[i+1];
                }
                continue;
            }
            if (arg_parse(argdef_color, argv[i]))
            {
                if (argv[i+1] && argv[i+1][0] != '-')
                {
                    accent_color_char = argv[i+1][0];
                }

                continue;
            }
            if (arg_parse(argdef_bold, argv[i]))
            {
                accent_bold = true;
                continue;
            }
            if (arg_parse(argdef_help, argv[i]))
            {
                help_menu_print(argdefs_list);
                return 0;
            }
        }
    }

    char *ascii = get_ascii(use_ascii_file, ascii_filename, ASCII_FILE_SIZE);
    char *distro_id = get_distro_id(DEFAULT_SIZE);
    char *uptime = get_uptime(DEFAULT_SIZE);
    char *kernel_version = get_kernel_version(DEFAULT_SIZE);

    struct SystemInfo system_info = {.ascii = ascii, .distro_id = distro_id, .kernel_version = kernel_version, .uptime = uptime};

    info_print(accent_color_char, accent_bold, system_info);
    
    free(ascii);
    free(distro_id);
    free(uptime);
    free(kernel_version);
    return 0;
}

bool arg_parse(const char *argdef[], const char *string)
{
    return strcmp(string, argdef[0]) == 0 || strcmp(string, argdef[1]) == 0;
}

void help_menu_print(const char **argdefs_list[])
{
    printf("usage: lowfetch [options]\n");
    for(int i = 0; i < ARGDEFS_LIST_SIZE; ++i)
    {
        printf("%s, %s,\t%s\n", argdefs_list[i][0], argdefs_list[i][1], argdefs_list[i][2]);
    }
}
#undef ARGDEFS_LIST_SIZE

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
    if (!output) 
    {
        fprintf(stderr, "error: unable to read '%s'\n", filename);
        return NULL;
    }
    else
    {
        output[strlen(output) - 1] = 0; // trim the trailing newline
    }
    fclose(file); // close the file since it's already been stored

    return output;
}

char *get_ansi_color_from(enum ColorChars color, bool bold)
{
    if (!bold)
    {
        switch (color)
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
        switch (color)
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

char *get_ascii(bool file_mode, const char *filename, size_t size)
{
    char *ascii;
    if (!file_mode || !file_read(filename, size))
    {
        ascii = malloc((size+1) * sizeof(*ascii));
        strcpy(ascii, ascii_default); // copy ascii_default into ascii
        return ascii;
    }
    else 
    {
        ascii = file_read(filename, size);
        return ascii;
    }
}

char *get_distro_id(size_t size)
{
    /* TODO: parser for /etc/os-release */
    FILE *file = fopen("/etc/os-release", "r");
    if (!file)
    {
        fprintf(stderr, "error: '/etc/os-release' is inaccessible\n");
        return NULL;
    }

    char *distro_id;
    distro_id = malloc((size+1)*sizeof(*distro_id));

    fscanf(file, "%s", distro_id);

    fclose(file);
    
    return distro_id;
}

char *get_uptime(size_t size)
{
    FILE *file = fopen("/proc/uptime", "r");
    if (!file)
    {
        fprintf(stderr, "error: uptime is inaccessible\n");
        return NULL;
    }

    char *uptime;
    unsigned int uptime_seconds = -1; // default value is integer limit, making errors obvious
    uptime = malloc((size+1)*sizeof(*uptime));

    fscanf(file, "%u", &uptime_seconds); // store value of file to an unsigned integer, flooring (rounding) it
    // format uptime string accordingly
    if (uptime_seconds >= 60)
    {
        switch (uptime_seconds%60) // switch seconds
        {
            case 0:
                sprintf(uptime, "%u minutes", uptime_seconds/60);
                break;
            case 1:
                sprintf(uptime, "%u minutes and 1 second", uptime_seconds/60);
                break;
            default:
                sprintf(uptime, "%u minutes and %u seconds", uptime_seconds/60, uptime_seconds%60);
        }
    }
    else
    {
        sprintf(uptime, "%u seconds", uptime_seconds);
    }

    fclose(file);

    return uptime;
}

char *get_kernel_version(size_t size)
{
    char *filename = "/proc/version";
    char *kernel_version = file_read(filename, size);
    return kernel_version;
}

int info_print(enum ColorChars accent_color_char, bool accent_bold, struct SystemInfo system_info)
{
    char *ansi_accent_color = get_ansi_color_from(accent_color_char, accent_bold);
    char order[] = {CHAR_ASCII, CHAR_DISTRO_ID, CHAR_KERNEL_VERSION, CHAR_UPTIME};

    for (int i = 0; i <= sizeof(order); ++i)
    {
        switch (order[i]) {
            case CHAR_ASCII:          printf("%s%s%s\n",         ansi_accent_color, system_info.ascii, ANSI_COLOR_RESET); break;
            case CHAR_DISTRO_ID:      printf("%sdistro:%s %s\n", ansi_accent_color, ANSI_COLOR_RESET,  system_info.distro_id); break;
            case CHAR_UPTIME:         printf("%suptime:%s %s\n", ansi_accent_color, ANSI_COLOR_RESET,  system_info.uptime); break;
            case CHAR_KERNEL_VERSION: printf("%skernel:%s %s\n", ansi_accent_color, ANSI_COLOR_RESET,  system_info.kernel_version);
        }
    }

    return 0;
}
