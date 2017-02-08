CURL_CFLAGS=$(shell curl-config --cflags)
GLIB_CFLAGS=$(shell pkg-config --cflags glib-2.0)

CURL_LIBS=$(shell curl-config --libs)
GLIB_LIBS=$(shell pkg-config --libs glib-2.0)

test_fetch: test_fetch.c
	$(CC) $(CURL_CFLAGS) $(GLIB_CFLAGS) -o test_fetch test_fetch.c $(CURL_LIBS) $(GLIB_LIBS)

clean:
	rm -f test_fetch
