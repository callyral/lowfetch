#include <stdlib.h>
#include "xdg_desktop.h"

char *get_xdg_desktop() {
    // this prefers $XDG_CURRENT_DESKTOP over $XDG_SESSION_DESKTOP
    if (!getenv("XDG_SESSION_DESKTOP") && !getenv("XDG_CURRENT_DESKTOP")) {
        return "unknown";
    }

    if (getenv("XDG_CURRENT_DESKTOP")) {
        return getenv("XDG_CURRENT_DESKTOP");
    }

    return getenv("XDG_SESSION_DESKTOP");
}
