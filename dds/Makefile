#-----------------------------------------------------------------------
#
#  Makefile for program dds
#
#-----------------------------------------------------------------------

SHELL	= /bin/bash
CC	= gcc
CFLAGS	= -O3 -DTICK=1.0e-3

EXEC	= ../bin/dds

OBJECTS	= dda.o

SOURCES	= 			\
	  bottomup_tree.c	\
	  build_node.c		\
	  build_scard.c		\
	  build_tree.c		\
	  build_trick.c		\
	  check_board.c		\
	  delete_tree.c		\
	  hashbackup.c		\
	  join_scard.c		\
	  play_board.c		\
	  play_board_start.c	\
	  print_board.c		\
	  print_node.c		\
	  print_scard.c		\
	  print_stat.c		\
	  print_tree.c		\
	  process_args.c	\
	  dds.c

${EXEC}: ${OBJECTS}
	${CC} ${CFLAGS} -o ${EXEC} ${OBJECTS}
	[[ -f ${EXEC}.exe ]] && mv ${EXEC}.exe ${EXEC}

.SUFFIXES:
.SUFFIXES: .o .c .h

.c.o:
	${CC} ${CFLAGS} -c $*.c

${OBJECTS}: Makefile header.h ${SOURCES}

clean:
	-@rm -f ${OBJECTS} > /dev/null 2>&1
