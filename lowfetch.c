#include "lowfetch.h"
#include "colors.h"
#define ASCII_FILE_SIZE 4096
#define DEFAULT_SIZE 256
/* color used when --color is used without an argument */
#define DEFAULT_COLOR ANSI_COLOR_BLUE
#define DEFAULT_COLOR_BOLD ANSI_COLOR_BLUE_BOLD

/* from https://www.asciiart.eu/animals/cats */
static char *ascii_default = " |\\'/-..--.\n"
                             " / _ _   ,  ;\n"
                             "`~=`Y'~_<._./\n"
                             " <`-....__.'";

enum ColorChars
{
  CHAR_WHITE = 'w',
  CHAR_RED = 'r',
  CHAR_GREEN = 'g',
  CHAR_YELLOW = 'y',
  CHAR_BLUE = 'b',
  CHAR_MAGENTA = 'm',
  CHAR_CYAN = 'c'
};

enum FileMode
{
    MODE_FILE_NONE,
    MODE_FILE
};

struct SystemInfo
{
    char *ascii;
    char *distro_id;
    char *uptime;
    char *kernel_version;
};

int main(int argc, char **argv)
{
    enum ColorChars accent_color = CHAR_WHITE;
    enum FileMode file_mode = MODE_FILE_NONE;
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
    
    /* CLI argument parsing (could be better though) */
    if (argc > 1)
    {
        int i;
        for (i = 1; i < argc; ++i)
        {
            if (strcmp(argv[i], "--file") == 0)
            {
                file_mode = MODE_FILE;
                /* set ascii_filename to what is after "--file", unless it starts with '-' */
                if (argv[i+1] && argv[i+1][0] != '-')
                {
                    ascii_filename = argv[i+1];
                }
                continue;
            }
            if (strcmp(argv[i], "--color") == 0)
            {
                accent_color = CHAR_BLUE;
                if (argv[i+1] && argv[i+1][0] != '-')
                {
                    accent_color = argv[i+1][0];
                }

                continue;
            }
            if (strcmp(argv[i], "--bold") == 0)
            {
                accent_bold = true;
                continue;
            }
        }
    }

    char *ascii = get_ascii(file_mode, ascii_filename, ASCII_FILE_SIZE);
    char *distro_id = get_distro_id(DEFAULT_SIZE);
    char *uptime = get_uptime(DEFAULT_SIZE);
    char *kernel_version = get_kernel_version(DEFAULT_SIZE);

    struct SystemInfo system_info = {ascii, distro_id, uptime, kernel_version};

    info_print(accent_color, accent_bold, system_info);
    
    free(ascii);
    //free(distro_id);
    free(uptime);
    free(kernel_version);
    return 0;
}

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
            case CHAR_MAGENTA:
                return ANSI_COLOR_MAGENTA;
            case CHAR_CYAN:
                return ANSI_COLOR_CYAN;
            default:
                return DEFAULT_COLOR;
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
            case CHAR_MAGENTA:
                return ANSI_COLOR_MAGENTA_BOLD;
            case CHAR_CYAN:
                return ANSI_COLOR_CYAN_BOLD;
            default:
                return DEFAULT_COLOR_BOLD;
        }
    }
}

char *get_ascii(enum FileMode file_mode, const char *filename, size_t size)
{
    char *ascii;
    switch (file_mode)
    {
        case MODE_FILE_NONE:
            ascii = malloc((size+1) * sizeof(*ascii));
            strcpy(ascii, ascii_default); // copy ascii_default into ascii
            return ascii;
        case MODE_FILE:
            ascii = file_read(filename, size);
            /* if ascii is NULL, use ascii_default instead */
            if (!ascii)
            {
                ascii = malloc((size+1) * sizeof(*ascii));
                strcpy(ascii, ascii_default);
                return ascii;
            }
            else
            {
                return ascii;
            }
    }
}

char *get_distro_id(size_t size)
{
    /* TODO: parser for /etc/os-release */
    //char *filename = "/etc/os-release";
    return "work in progress!!!";
}

char *get_uptime(size_t size)
{
    char *filename = "/proc/uptime";
    char *uptime = file_read(filename, size);
    return uptime;
}

char *get_kernel_version(size_t size)
{
    char *filename = "/proc/version";
    char *kernel_version = file_read(filename, size);
    return kernel_version;
}



#define GET_COLOR() get_ansi_color_from(accent_color, accent_bold)
int info_print(enum ColorChars accent_color, bool accent_bold, struct SystemInfo system_info)
{
    /* TODO: add bold options */
    printf("%s%s%s\n", GET_COLOR(), system_info.ascii, ANSI_COLOR_RESET);
    printf("%sdistro:%s %s\n", GET_COLOR(), ANSI_COLOR_RESET, system_info.distro_id);
    printf("%suptime:%s %s\n", GET_COLOR(), ANSI_COLOR_RESET, system_info.uptime);
    printf("%skernel:%s %s\n", GET_COLOR(), ANSI_COLOR_RESET, system_info.kernel_version);

    return 0;
}
#undef GET_ANSI
