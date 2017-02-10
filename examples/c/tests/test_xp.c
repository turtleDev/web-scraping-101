#include "xp.h"


char *document = ""
"<!doctype html>"
"<head>"
"  <title>damn son, where'd you find this?</title>"
"</head>"
"<body>"
"  <div class='one'>"
"    <p>this is a test</p>"
"    <p>this should work</p>"
"  </div>"
"</body>";

int main()
{
    xp_init();
    struct xp_context *doc = xp_context_new(document);
    struct xp *result = xp_exec(doc, "//p");
    int i;
    for ( i = 0; i < result->len; ++i ) {
        printf("%s\n", result->nodes[i]->name);
    }

    xp_context_free(doc);
    xp_free(result);
    xp_cleanup();
    return 0;
}

