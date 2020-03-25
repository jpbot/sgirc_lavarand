#!/usr/bin/make
#
# makefile for cap_sha1

# standard tools
#
SHELL= /bin/sh
TR= tr
DIFF= diff

# select the proper compiler
#
# If you use a non-ANSI cc, you may have to edit md5.c to change the
# UL constant suffixes in MD5Transform() to just L and see if test works.
#
# Under BSD/OS 2.0, we cannot compile with -ansi because <sys/resource.h>
# uses struct timeval which is not defined by <sys/time.h> under -ansi.
#
CC= cc
#CC= gcc -ansi
#CC= gcc -ansi -fcaller-saves
#CC= gcc2 -ansi -fcaller-saves
#CC= gcc
#CC= gcc -fcaller-saves
#CC= gcc2 -fcaller-saves

# select the proper optimization or debug level
#
#OPT_DEBUG=
#OPT_DEBUG= -DDEBUG
#OPT_DEBUG= -g3
#OPT_DEBUG= -g3 -DDEBUG
#OPT_DEBUG= -g3 -DDEBUG -DDETAILED_DEBUG
#OPT_DEBUG= -O
#OPT_DEBUG= -O -DDEBUG
#OPT_DEBUG= -O -g3
#OPT_DEBUG= -O -g3 -DDEBUG
#OPT_DEBUG= -O2
#OPT_DEBUG= -O2 -DDEBUG
#OPT_DEBUG= -O2 -g3
#OPT_DEBUG= -O2 -g3 -DDEBUG
#OPT_DEBUG= -O3
#OPT_DEBUG= -O3 -DDEBUG
OPT_DEBUG= -O3 -g3
#OPT_DEBUG= -O3 -g3 -DDEBUG

CFLAGS= ${OPT_DEBUG} -Wall 
#CFLAGS= ${OPT_DEBUG} -Wall -W -Werror

# Some architectures such as Sparc do not allow one to access long that
# are not alligned.  If make test dumpds core or fails to produce no
# output, use only -DMUST_ALIGN.
#
# ALIGN=		let must_align.c figure out if alignment is needed
# ALIGN=-DMUST_ALIGN 	force alignment (at the cost of performance}
# ALIGN=-UMUST_ALIGN	allow non-aligment (usually at a performance gain}
#
ALIGN=
#ALIGN=-DMUST_ALIGN
#ALIGN=-UMUST_ALIGN

all: cap_sha1

# shs/shs1drvr.o shs/shs1io.o shs/shs1.o
cap_sha1: cap_sha1.o
	${CC} ${CFLAGS} -o cap_sha1 cap_sha1.o sha1/shs1drvr.o sha1/shs1io.o sha1/shs1.o -lvl

cap_sha1.o: cap_sha1.c 
# shs/shs1.h
	${CC} ${CFLAGS} -c cap_sha1.c

clean:
	rm -f cap_sha1.o

clobber: clean
	rm -f cap_sha1
