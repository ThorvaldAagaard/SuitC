#-----------------------------------------------------------------------
#  Makefile for program playc
#-----------------------------------------------------------------------

SHELL	= /bin/sh
CPFLAGS	=
CFLAGS	= -O3
CC	= gcc
LDFLAGS	=
LD	= gcc

OBJECTS	= playcode.o

SOURCES	= create_all_boards.c	\
	  determine_sequences.c	\
	  playc.c		\
	  print_boards.c

EXEC	= ../bin/playc

.SUFFIXES:
.SUFFIXES: .o .c

${EXEC}: ${OBJECTS}
	${LD} ${LDFLAGS} -o ${EXEC} ${OBJECTS}
	[[ -f ${EXEC}.exe ]] && mv ${EXEC}.exe ${EXEC}

.SUFFIXES:
.SUFFIXES: .o .c .h

.c.o:
	${CC} ${CFLAGS} -c $*.c

${OBJECTS}: Makefile header.h ${SOURCES}

clean:
	-@rm -f ${OBJECTS} > /dev/null 2>&1
