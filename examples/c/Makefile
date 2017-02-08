CFLAGS=$(shell pkg-config --cflags glib-2.0)
LIBS=$(shell pkg-config --libs glib-2.0)

hn: hn.c
	@ gcc $(CFLAGS) $(LIBS) -o hn hn.c
