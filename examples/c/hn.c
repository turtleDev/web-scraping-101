/**
 * hn.c -- C implementation of the Hacker News Crawler
 *
 * We're on the dark side now. All nice abstractions are gone.
 * No more safety of a complacent interpreter.
 *
 * Now we battle the ruthless compiler and level up!
 */

#include <stdio.h>
#include <stdbool.h>
#include <glib.h>

#include "debug.h"

/**
 * make a network request
 */
char *fetch(const char *url)
{

    check(0, "http error");
    return NULL;
error:
    return NULL;
}

int init() { return true; }
int cleanup() { }

int main()
{
    check(init(), "can't init");

    const char *url = "http://news.ycombinator.com";

    char *body = fetch(url);
    check(body, "fetch failed");

    // don't really care if clean up failed; OS nuke everything anyway
    cleanup();
    return 0;

error:
    return -1;
}
