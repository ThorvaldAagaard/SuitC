#-----------------------------------------------------------------------
#
#  Makefile for program SuitC
#
#  This is a restart from ExpertPlay Rev6.102
#
#  Copyright 2020, Gerrit A. van der Velde, Leiden
#
#-----------------------------------------------------------------------

SHELL	= /bin/bash
CC	= gcc -O3

EXEC	= ../bin/SuitC
EXEC1	= ../bin/sc
EXEC2	= ../bin/ExpertPlay

OBJECTS = suitall.o

SOURCES =				\
	  add_node.c			\
	  add_quirkd.c			\
	  build_selected_tree.c		\
	  check_ew_plays.c		\
	  check_ns_plays.c		\
	  check_quirkd.c		\
	  check_zermelo.c		\
	  clean_quirkd.c		\
	  convert_tree_to_quirk.c	\
	  copy_node.c			\
	  copy_tree.c			\
	  count_plays.c			\
	  count_quirkd.c		\
	  create_all_boards.c		\
	  create_board_header.c		\
	  create_boards.c		\
	  create_ply1.c			\
	  create_ply2.c			\
	  create_ply3.c			\
	  create_ply4.c			\
	  create_tricks.c		\
	  delete_board.c		\
	  delete_quirkd.c		\
	  delete_tree.c			\
	  determine_trick_count.c	\
	  evaluate_best_results.c	\
	  evaluate_mp_results.c		\
	  evaluate_mp_scores.c		\
	  fold_ew_tree.c		\
	  fold_ns_tree.c		\
	  fold_quirkd.c			\
	  limit_matchpoint_nodes.c	\
	  limit_quirkd.c		\
	  merge_nodes.c			\
	  merge_quirkd.c		\
	  merge_tree.c			\
	  minimax.c			\
	  print_all_results.c		\
	  print_boards.c		\
	  print_node.c			\
	  print_play_count.c		\
	  print_quirkd.c		\
	  print_results.c		\
	  print_summary_results.c	\
	  print_tree.c			\
	  process_args.c		\
	  select_ew_plays.c		\
	  select_ns_plays.c		\
	  sort_plays.c			\
	  suitc.c			\

${EXEC}: ${OBJECTS}
	${CC} -o ${EXEC} ${OBJECTS}
	[[ -f ${EXEC}.exe ]] && mv ${EXEC}.exe ${EXEC} || true
	rm -f ${EXEC1}; ln -s ${EXEC} ${EXEC1}
	rm -f ${EXEC2}; ln -s ${EXEC} ${EXEC2}

.SUFFIXES:
.SUFFIXES: .o .c .h

.c.o:
	${CC} -c $*.c

${OBJECTS}:	Makefile	header.h	${SOURCES}

clean:
	-@rm -f ${OBJECTS} > /dev/null 2>&1
