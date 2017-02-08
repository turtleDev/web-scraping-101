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
#include <libxml/tree.h>

#include "fetch.h"
#include "xp.h"
#include "debug.h"

int index_at_occurance(char *str, char c, int o)
{
    int i;
    int len = strlen(str);
    for ( i = 0; i < len; ++i ) {
        if ( str[i] == c ) { --o; }
        if ( o == 0 ) { return i; }
    }

    return -1;
}

bool init() 
{ 
    return fetch_init() && xp_init();
}


bool cleanup() 
{
    return fetch_cleanup() && xp_cleanup();
}

char *scrape(char *body)
{
    struct xp_object *doc = xp_doc_new(body);
    struct xp_list *titles = xp_exec(doc, "//a[@class='storylink']");
    int i;
    for ( i = 0; i < titles->len; ++i ) {
        xmlNodePtr car;
        car = titles->nodes[i]->children;
        while(car) {
            if ( car->type == XML_TEXT_NODE ) {
                printf("%s\n", car->content);
            }
            car = car->next;
        }
    }

    // find the next hypertext reference, if any.
    char *next_href = NULL;
    struct xp_list *href = xp_exec(doc, "//a[@class='morelink']/@href");
    if ( href->nodes ) {

        xmlNodePtr tmp = href->nodes[0]->children;

        while(tmp) {
            if ( tmp->type == XML_TEXT_NODE ) {
                next_href = strdup((const char *)tmp->content);
            }
            tmp = tmp->next;
        }
    }


    xp_doc_free(doc);
    xp_list_free(titles);
    xp_list_free(href);


    return next_href;
}


char *next_url(char* current_url, char *href)
{
    /**
      * find the third occurance of character '/',
      * basically we want to find the end of the domain name
      */
    int limit = index_at_occurance(current_url, '/', 3);
    char *base = strndup(current_url, limit);
    char *url = g_strjoin(NULL, base, "/", href, NULL);
    free(current_url);
    free(base);
    return url;
}

bool crawl()
{
    char *url = strdup("https://news.ycombinator.com");
    char *next_href = NULL;
    char *body = NULL;

    while(url) {

        body = fetch(url);
        check(body, "error retrieving %s", url);

        next_href = scrape(body);
        free(body);

        if ( next_href ) {
            url = next_url(url, next_href);
        } else {
            free(url);
            url = NULL;
        }

        if (next_href) free(next_href);
    }

    return true;
error:
    return false;
}

int main()
{
    check(init(), "can't init");

    check(crawl(), "there was an error with the crawler");

    // don't really care if clean up failed; OS will nuke everything anyway
    cleanup();
    return 0;

error:
    cleanup();
    return -1;
}
