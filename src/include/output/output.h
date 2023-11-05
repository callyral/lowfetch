#ifndef OUTPUT_H_
#define OUTPUT_H_
#include <stdbool.h>
 
#define CHAR_ASCII          'a'
#define CHAR_DISTRO_ID      'd'
#define CHAR_KERNEL_VERSION 'k'
#define CHAR_PACKAGE_AMOUNT 'p'
#define CHAR_SHELL          's'
#define CHAR_UPTIME         'u'
#define CHAR_XDG_DESKTOP    'x'

typedef struct {
    char *ascii;
    char *distro_id;
    char *kernel_version;
    char *package_amount;
    char *shell;
    char *uptime;
    char *xdg_desktop;
}SystemInfo;

void info_print(char color_char, bool bold, bool use_order_file, char *order_filename, SystemInfo system_info);

#endif
