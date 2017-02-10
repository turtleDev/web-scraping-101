
#include <stdio.h>
#include <glib.h>

#include "fetch.h"
#include "debug.h"
#include "misc.h"

int main()
{
    fetch_init();
    GString *str = g_string_new(NULL);
    GString *url = g_string_new("http://example.com");

    if ( fetch(url, str) == HN_OK ) {
        printf("%s\n", GSTR(str));
    } else {
        printf("something went wrong\n");
    }

    g_string_free(str, true);
    g_string_free(url, true);

    fetch_cleanup();
    return 0;
}
