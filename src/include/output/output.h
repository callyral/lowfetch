#ifndef OUTPUT_H_
#define OUTPUT_H_
#include <stdbool.h>
#include <stdlib.h>
 
#define CHAR_ASCII          'a'
#define CHAR_DISTRO_ID      'd'
#define CHAR_KERNEL_VERSION 'k'
#define CHAR_PACKAGE_AMOUNT 'p'
#define CHAR_SHELL          's'
#define CHAR_UPTIME         'u'
#define CHAR_XDG_DESKTOP    'x'

struct SystemInfo
{
    char *ascii;
    char *distro_id;
    char *kernel_version;
    char *package_amount;
    char *shell;
    char *uptime;
    char *xdg_desktop;
};

void info_print(char accent_color_char, bool accent_bold, bool use_order_file, char *order_filename, size_t order_filesize, struct SystemInfo system_info);

#endif
