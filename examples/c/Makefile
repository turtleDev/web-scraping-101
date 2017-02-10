CURL_CFLAGS=$(shell curl-config --cflags)
GLIB_CFLAGS=$(shell pkg-config --cflags glib-2.0)
LXML_CFLAGS=$(shell xml2-config --cflags)


CURL_LIBS=$(shell curl-config --libs)
GLIB_LIBS=$(shell pkg-config --libs glib-2.0)
LXML_LIBS=$(shell xml2-config --libs)

CFLAGS= -g -Wall $(CURL_CFLAGS) $(GLIB_CFLAGS) $(LXML_CFLAGS)
LIBS=$(CURL_LIBS) $(GLIB_LIBS) $(LXML_LIBS)

hn: hn.c
	$(CC) $(CFLAGS) $(LIBS) -o hn hn.c

clean:
	rm -f hn

.PHONY: clean
