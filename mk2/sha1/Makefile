#!/usr/bin/make
#
# hash - combined shs and md5 makefile
#
# @(#) $Revision: 13.5 $
# @(#) $Id: Makefile,v 13.5 2010/10/12 21:08:53 chongo Exp $
# @(#) $Source: /usr/local/src/cmd/hash/RCS/Makefile,v $
#
# This file was written by Landon Curt Noll.
#
# This makefile has been placed in the public domain.  Please do not
# copyright this makefile.
#
# LANDON CURT NOLL DISCLAIMS ALL WARRANTIES WITH  REGARD  TO
# THIS  SOFTWARE,  INCLUDING  ALL IMPLIED WARRANTIES OF MER-
# CHANTABILITY AND FITNESS.  IN NO EVENT SHALL  LANDON  CURT
# NOLL  BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
# DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM  LOSS  OF
# USE,  DATA  OR  PROFITS, WHETHER IN AN ACTION OF CONTRACT,
# NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR  IN
# CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#
# chongo (was here) /\oo/\
# http://www.isthe.com/chongo/index.html
#
# Share and enjoy! :-)
#
# See shsdrvr.c, shs1drvr.c and md5drvr.c for version and modification history.

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

all: sha1

sha1: shs1drvr.o shs1io.o shs1dual.o shs1.o
	${CC} ${CFLAGS} -o sha1 shs1drvr.o shs1io.o shs1dual.o shs1.o

# perform the shs1 test
sha1test: sha1 file1 file2 file3 shs1.data shs1.ddata mega Makefile
	@echo This shs1 test should produce no output
	@./sha1 -x | ${DIFF} -u - shs1.data
	@./sha1 shs1.data | \
	    sed '/^407a65c1b4c95c5dd19da3c22b5d7357eadc2aeb shs1.data$$/d'
	@./mega | ./sha1 | \
	    sed '/^34aa973cd4c4daa4f61eeb2bdbad27316534016f$$/d'
	@./sha1 -q -c file3 | \
	    sed '/^0xa2ecae1924928cfebf7bbb01ee0974063e845824$$/d'
	@./sha1 -p abc -c file2 | \
	    sed '/^0xa2ecae1924928cfebf7bbb01ee0974063e845824 file2$$/d'
	@./sha1 -P file1 file2 | \
	    sed '/^a2ecae1924928cfebf7bbb01ee0974063e845824 file2$$/d'
	@./sha1 shs1.ddata | \
	    sed '/^a32cb7757f15c922f5466c279f12803d5848c21b shs1.ddata$$/d'
	@./sha1 -d -x -q | ${DIFF} -u - shs1.ddata
	@echo End of sha1 test


file1: Makefile
	rm -f file1
	echo abc | ${TR} -d '\012' > file1

file3: file1 file2
	rm -f file3
	cat file1 file2 > file3

shs1drvr.o: shs1drvr.c shs1.h Makefile
	${CC} ${CFLAGS} -DTLIB=\"${SHSLIB}\" -c shs1drvr.c

shs1io.o: shs1io.c shs1.h
	${CC} ${CFLAGS} -c shs1io.c

shs1dual.o: shs1dual.c shs1.h Makefile
	${CC} ${CFLAGS} -DTLIB=\"${SHSLIB}\" -c shs1dual.c

shs1.o: shs1.c shs1.h align.h endian.h
	${CC} ${CFLAGS} -c shs1.c

align.h: must_align
	rm -f align.h
	-./must_align > align.h 2>/dev/null

must_align: must_align.c Makefile
	${CC} ${ALIGN} must_align.c -o must_align

endian.h: endian
	rm -f endian.h
	./endian > endian.h

endian: endian.c
	${CC} ${CFLAGS} endian.c -o endian

mega: mega.c
	${CC} ${CFLAGS} mega.c -o mega

clean:
	rm -f shs1.o shs1drvr.o shs1dual.o mega mega.o
	rm -f shs1io.o
	rm -f syscrypt.o
	rm -f file1 file3
	rm -f endian.h endian.o endian
	rm -f align.h must_align must_align.o
	rm -f core core.must_align core.shs core.md5

clobber: clean
	rm -f sha1
	rm -rf endian.dSYM mega.dSYM
