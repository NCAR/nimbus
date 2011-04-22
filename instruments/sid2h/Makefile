#
# SID2H Makefile.  This only has an install target.
#
BINDIR=	${JLOCAL}/bin
LIBDIR=	${JLOCAL}/idl/lib
MANDIR=	${JLOCAL}/man/man1

PROG=	sid2h

${PROG}:

install:
	test -d $(BINDIR) || mkdir $(BINDIR)
	cp ${PROG} ${BINDIR}
	test -d $(LIBDIR) || mkdir $(LIBDIR)
	cp *.pro ${LIBDIR}

