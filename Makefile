CC?=gcc
CFLAGS+=-Wall -pedantic -std=c89

web-blocker: web-blocker.c
	$(CC) $(CFLAGS) -o web-blocker web-blocker.c

clean:
	rm -f web-blocker

install: web-blocker
	install -D -m 755 web-blocker ${DESTDIR}${PREFIX}/bin/web-blocker

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/web-blocker

.PHONY: clean install uninstall
