#include <stdlib.h>

char *get_desktop_session() {
    // this prefers $XDG_CURRENT_DESKTOP over $XDG_SESSION_DESKTOP
    if (!getenv("XDG_SESSION_DESKTOP") && !getenv("XDG_CURRENT_DESKTOP")) {
        return "unknown";
    }

    if (getenv("XDG_CURRENT_DESKTOP")) {
        return getenv("XDG_CURRENT_DESKTOP");
    }

    return getenv("XDG_SESSION_DESKTOP");
}
