#include "../fetch.h"

int main()
{
    fetch_init();

    char *data = fetch("http://example.com");
    printf("%s\n", data);
    free(data);

    fetch_cleanup();
    return 0;
}
