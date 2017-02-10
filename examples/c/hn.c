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
#include <time.h>

#include <glib.h>
#include <libxml/tree.h>

#include "xp.h"
#include "debug.h"
#include "fetch.h"
#include "misc.h"

#define START_URL "https://news.ycombinator.com"

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

int init() 
{ 
    if ( fetch_init() == HN_OK && xp_init() == HN_OK ) 
        return HN_OK;
    else
        return HN_ERR;

}

bool cleanup() 
{
    return fetch_cleanup() && xp_cleanup();
}

void process(GHashTable *obj)
{
    if ( !g_hash_table_lookup(obj, "user") ) {
        g_hash_table_insert(obj, "user", strdup("(N/A)"));
    }

    GHashTableIter iter;
    g_hash_table_iter_init(&iter, obj);
    gpointer key;
    gpointer val;

    /**
     * I'm just rendering the hash table in JSON-like format.
     * even though it's not exactly the same, but you could take
     * the hashtable object and then serialise it into a real
     * JSON string and then log it to console or write it to a file.
     */
    printf("{\n");
    while(g_hash_table_iter_next(&iter, &key, &val)) {
        printf("    \"%s\": \"%s\"\n", (char *)key, (char *)val);
    }
    printf("}\n");
}

gboolean hash_table_clear(gpointer key, gpointer value, gpointer udata)
{
    if ( value ) free(value);
    return true;
}

void scrape(GString *body, GString OUT *next_href)
{
    struct xp_object *doc = xp_doc_new(GSTR(body));
    struct xp_list *storylinks = xp_exec(doc, "//table[@class='itemlist']//td[@class='title']/a[@class='storylink']");
    struct xp_list *subtexts = xp_exec(doc, "//table[@class='itemlist']//td[@class='subtext']");

    int min_length;
    if ( storylinks->len < subtexts->len ) {
        min_length = storylinks->len;
    } else {
        min_length = subtexts->len;
    }

    int i;
    GHashTable *obj = g_hash_table_new(g_str_hash, g_str_equal);
    for ( i = 0; i < min_length; ++i ) {


        xmlNodePtr head = storylinks->nodes[i];
        xmlNodePtr opt = subtexts->nodes[i];
        
        xmlAttrPtr attr_tmp;
        xmlNodePtr tmp; 

        // extract the text of the post
        tmp = head->children;
        while ( tmp ) {
            if ( tmp->type == XML_TEXT_NODE ) {
                g_hash_table_insert(obj, "name", strdup((const char *)tmp->content));
            }
            tmp = tmp->next;
        }

        // extract the link
        attr_tmp = head->properties;
        while(attr_tmp) {
            if ( !strcmp((const char *)attr_tmp->name, "href") ) {
                tmp = attr_tmp->children;
                while(tmp) {
                    if ( tmp->type == XML_TEXT_NODE ) {
                        // make sure it leads to a valid domain
                        if ( !strncmp((const char *)tmp->content, "http", 4) ) {
                            g_hash_table_insert(obj, "url", strdup((const char *)tmp->content));
                        } else {
                        // ... else it's probably a self post
                            char *link = g_strjoin(NULL, START_URL, "/", tmp->content, NULL);
                            g_hash_table_insert(obj, "url", link);
                        }
                    }
                    tmp = tmp->next;
                }
            }
            attr_tmp = attr_tmp->next;
        }

        // user's name
        tmp = opt->children;
        while(tmp) {
            if ( tmp->type == XML_ELEMENT_NODE && !strcmp((const char *)tmp->name, "a") ) {
                attr_tmp = tmp->properties;
                while(attr_tmp) {
                    if ( !strcmp((const char *)attr_tmp->name, "class") ) {
                        xmlNodePtr class = attr_tmp->children;
                        while(class) {
                            if ( class->type == XML_TEXT_NODE && !strcmp((const char *)class->content, "hnuser") ) {
                                xmlNodePtr val = tmp->children;
                                while(val) {
                                    if ( val->type == XML_TEXT_NODE ) {
                                        g_hash_table_insert(obj, "user", strdup((const char *)val->content));
                                    }
                                    val = val->next;
                                }
                            }
                            class = class->next;
                        }
                    }
                    attr_tmp = attr_tmp->next;
                }
            }
            tmp = tmp->next;
        }

        // the link to the comments is the last node in the element
        tmp = opt->children;
        while(tmp) {
            if ( !strcmp((const char *)tmp->name, "a") ) {
                attr_tmp = tmp->properties;
                while(attr_tmp) {
                    if ( !strcmp((const char *)attr_tmp->name, "href") ) {
                        xmlNodePtr val = attr_tmp->children;
                        while(val) {
                            if ( val->type == XML_TEXT_NODE && !strncmp((const char *)val->content, "item", 4) ) {
                                char *link = g_strjoin(NULL, START_URL, "/", val->content, NULL);
                                g_hash_table_insert(obj, "comments", link);
                            }
                            val = val->next;
                        }
                    }
                    attr_tmp = attr_tmp->next;
                }
            }
            tmp = tmp->next;
        }

        // log the crawl time
        time_t now;
        time(&now);
        char *tstr = strdup(ctime(&now));
        int len = strlen(tstr);
        tstr[len-1] = '\0'; // this gets rid of the new line at the end
        g_hash_table_insert(obj, "collected_at", tstr);
        

        process(obj);

        // empty the object
        g_hash_table_foreach_remove(obj, hash_table_clear, NULL);

    }
    // free up resources
    g_hash_table_destroy(obj);
    xp_list_free(storylinks);
    xp_list_free(subtexts);

    struct xp_list *href = xp_exec(doc, "//a[@class='morelink']/@href");
    if ( href->nodes ) {

        xmlNodePtr tmp = href->nodes[0]->children;

        while(tmp) {
            if ( tmp->type == XML_TEXT_NODE ) {
                g_string_append(next_href,(const char *)tmp->content);
            }
            tmp = tmp->next;
        }
    }


    xp_list_free(href);
    xp_doc_free(doc);
}


void next_url(GString *url, GString IN OUT *href)
{
    /**
      * find the third occurance of character '/',
      * basically we want to find the end of the domain name
      */
    int limit = index_at_occurance(GSTR(url), '/', 3);
    char *base = strndup(GSTR(url), limit);
    char *next_url = g_strjoin(NULL, base, "/", GSTR(href), NULL);
    g_string_assign(url, next_url);
    free(base);
    free(next_url);
}

int crawl(const char *target)
{
    GString *url = g_string_new(target);
    GString *next_href = g_string_new(NULL);
    GString *body = g_string_new(NULL);

    while(true) {

        check(fetch(url, body) == HN_OK, "fetch() failed");
        scrape(body, next_href);
        next_url(url, next_href);

        if (!GSTR_LEN(next_href)) {
            break;
        }

        g_string_set_size(next_href, 0);
        g_string_set_size(body, 0);
    }

    g_string_free(url, true);
    g_string_free(next_href, true);
    g_string_free(body, true);

    return HN_OK;
error:
    g_string_free(url, true);
    g_string_free(next_href, true);
    g_string_free(body, true);

    return HN_ERR;
}

int main()
{
    check(init() == HN_OK, "can't init");

    check(crawl(START_URL) == HN_OK, "there was an error with the crawler");

    // don't really care if clean up failed; OS will nuke everything anyway
    cleanup();
    return 0;

error:
    cleanup();
    return -1;
}
