# charge — Mani Linux package manager
CC     ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -O2 -g
LDLIBS  = -larchive -lzstd -lcrypto -lcurl
PREFIX ?= /usr/local

OBJS = db.o pkg.o install.o repo.o util.o

all: charge recharge

charge: main.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

recharge: recharge.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c charge.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o charge recharge

install: all
	install -Dm755 charge $(DESTDIR)$(PREFIX)/bin/charge
	install -Dm755 recharge $(DESTDIR)$(PREFIX)/bin/recharge

.PHONY: all clean install