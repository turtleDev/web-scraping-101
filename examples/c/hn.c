/**
 * hn.c -- C implementation of the Hacker News Crawler
 *
 * We're on the dark side now. All nice abstractions are gone.
 * No more safety of a complacent interpreter.
 *
 * Now we battle the ruthless compiler
 *
 * To arms! my ... keyboard bashing brethren!
 */

#include <stdio.h>
#include <stdbool.h>
#include <glib.h>

#include "fetch.h"
#include "xp.h"
#include "debug.h"

bool init() 
{ 
    return fetch_init() && xp_init();
}


bool cleanup() 
{
    return fetch_cleanup() && xp_cleanup();
}

int main()
{
    check(init(), "can't init");

    char *url = "http://news.ycombinator.com";

    char *body = fetch(url);
    check(body, "fetch failed");

    // don't really care if clean up failed; OS will nuke everything anyway
    cleanup();
    return 0;

error:
    if (body) free(body);
    return -1;
}
