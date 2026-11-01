#-----------------------------------------------------------------------
#
#  Makefile for program dds
#
#  This is a restart of Rev1.036
#
#  Copyright 2026, Gerrit A. van der Velde, Leiden
#
#-----------------------------------------------------------------------

SHELL	= /bin/bash
CC	= gcc
CFLAGS	= -O3

EXEC	= ../bin/dds

OBJECTS	= 			\
	  bottomup_tree.o	\
	  build_node.o		\
	  build_ply.o		\
	  build_seqc.o		\
	  build_tree.o		\
	  check_board.o		\
	  delete_tree.o		\
	  hashbackup.o		\
	  hashseqc.o		\
	  join_scard.o		\
	  order_plays.o		\
	  play_board.o		\
	  play_board_start.o	\
	  print_all_boards.o	\
	  print_all_results.o	\
	  print_board.o		\
	  print_node.o		\
	  print_scard.o		\
	  print_seqc_tree.o	\
	  print_stat.o		\
	  print_tree.o		\
	  process_args.o	\
	  dds.o

${EXEC}: ${OBJECTS}
	${CC} ${CFLAGS} -o ${EXEC} ${OBJECTS}
	[[ -f ${EXEC}.exe ]] && mv ${EXEC}.exe ${EXEC} || true

.SUFFIXES:
.SUFFIXES: .o .c .h

extern.h: global.h
	sed '/^[cCdils]/s/^/extern /' global.h > extern.h

.c.o:
	${CC} ${CFLAGS} -c $*.c

${OBJECTS}:	Makefile	extern.h	header.h

clean:
	-@rm -f ${OBJECTS} > /dev/null 2>&1
