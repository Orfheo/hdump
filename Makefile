##############################################################################
# Makefile for hdump Utility
#
# 10/21/93  GVT  First Implementation
##############################################################################

CC=cc
CCFLAGS= -g -pg
LDFLAGS= -g -pg

INSTALL=install
DESTDIR=/usr/local
DESTBIN=$(DESTDIR)/bin
DESTMAN=$(DESTDIR)/share/man/man1

all : hdump

clean :
	rm -f hdump hdump.o ebcdic.o

install :
	$(INSTALL) -c -o bin -g bin hdump $(DESTBIN)
	$(INSTALL) -c -o bin -g bin -m 0444 hdump.man $(DESTMAN)/hdump.1

ebcdic.o : ebcdic.c
	$(CC) $(CCFLAGS) -c ebcdic.c

hdump.o : hdump.c ebcdic.h
	$(CC) $(CCFLAGS) -c hdump.c

hdump : hdump.o ebcdic.o
	$(CC) $(LDFLAGS) -o hdump hdump.o ebcdic.o
