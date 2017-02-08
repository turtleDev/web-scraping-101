#include "../xp.h"


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
    struct xp_object *doc = xp_doc_new(document);
    struct xp_list *result = xp_exec(doc, "//p");
    int i;
    for ( i = 0; i < result->len; ++i ) {
        printf("%s\n", result->nodes[i]->name);
    }
    return 0;
}

