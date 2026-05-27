CC      = gcc
CFLAGS  = -O2 -Wall -Wextra
PREFIX  = /usr/local
BINDIR  = $(PREFIX)/bin
DATADIR = $(PREFIX)/share/myfetch
MANDIR  = $(PREFIX)/share/man/man1

.PHONY: all install uninstall clean

all: myfetch

myfetch: myfetch.c
	$(CC) $(CFLAGS) myfetch.c -o myfetch

install: myfetch
	install -Dm755 myfetch        $(DESTDIR)$(BINDIR)/myfetch
	install -Dm644 ascii.txt      $(DESTDIR)$(DATADIR)/ascii.txt
	install -Dm644 myfetch.1      $(DESTDIR)$(MANDIR)/myfetch.1

uninstall:
	rm -f  $(DESTDIR)$(BINDIR)/myfetch
	rm -f  $(DESTDIR)$(DATADIR)/ascii.txt
	rm -f  $(DESTDIR)$(MANDIR)/myfetch.1
	rmdir --ignore-fail-on-non-empty $(DESTDIR)$(DATADIR) 2>/dev/null || true

clean:
	rm -f myfetch
