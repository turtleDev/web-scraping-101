CURL_CFLAGS=$(shell curl-config --cflags)
GLIB_CFLAGS=$(shell pkg-config --cflags glib-2.0)
LXML_CFLAGS=$(shell xml2-config --cflags)


CURL_LIBS=$(shell curl-config --libs)
GLIB_LIBS=$(shell pkg-config --libs glib-2.0)
LXML_LIBS=$(shell xml2-config --libs)

test_fetch: test_fetch.c
	$(CC) $(CURL_CFLAGS) $(GLIB_CFLAGS) -o test_fetch test_fetch.c $(CURL_LIBS) $(GLIB_LIBS)

test_xp: test_xp.c
	$(CC) $(LXML_CFLAGS) -o test_xp test_xp.c $(LXML_LIBS)

clean:
	rm -f test_fetch test_xp
