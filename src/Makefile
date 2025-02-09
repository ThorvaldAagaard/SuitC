#-----------------------------------------------------------------------
#
#  Makefile for program SuitC
#
#  This is a restart from SuitC Rev1.050
#
#  Copyright 2025, Gerrit A. van der Velde, Leiden
#
#-----------------------------------------------------------------------

SHELL	= /bin/bash
CC	= gcc -O3 -DSUITCAPP=NO

EXEC	= ../bin/SuitC
EXEC1	= ../bin/sc
EXEC2	= ../bin/ExpertPlay

OBJECTS =				\
	  add_node.o			\
	  add_quirk.o			\
	  build_selected_tree.o		\
	  check_ew_plays.o		\
	  check_ns_plays.o		\
	  check_quirk.o			\
	  check_zermelo.o		\
	  clean_quirk.o			\
	  convert_tree_to_quirk.o	\
	  copy_node.o			\
	  copy_tree.o			\
	  count_quirk.o			\
	  create_all_boards.o		\
	  create_board_header.o		\
	  create_boards.o		\
	  create_ply1.o			\
	  create_ply2.o			\
	  create_ply3.o			\
	  create_ply4.o			\
	  create_tricks.o		\
	  delete_board.o		\
	  delete_quirk.o		\
	  delete_tree.o			\
	  determine_trick_count.o	\
	  evaluate_best_results.o	\
	  evaluate_mp_results.o		\
	  evaluate_mp_scores.o		\
	  fold_ew_tree.o		\
	  fold_ns_tree.o		\
	  fold_quirk.o			\
	  fold_tree.o			\
	  limit_matchpoint_nodes.o	\
	  limit_quirk.o			\
	  merge_nodes.o			\
	  merge_quirk.o			\
	  minimax.o			\
	  print_all_results.o		\
	  print_boards.o		\
	  print_node.o			\
	  print_quirk.o			\
	  print_results.o		\
	  print_stat.o			\
	  print_suitc.o			\
	  print_summary_results.o	\
	  print_tree.o			\
	  process_args.o		\
	  select_ns_plays.o		\
	  use_suit_entries.o		\
	  suitc.o

${EXEC}: ${OBJECTS}
	${CC} -o ${EXEC} ${OBJECTS}
	[[ -f ${EXEC}.exe ]] && mv ${EXEC}.exe ${EXEC} || true
	rm -f ${EXEC1}; ln -s ${EXEC} ${EXEC1}
	rm -f ${EXEC2}; ln -s ${EXEC} ${EXEC2}

.SUFFIXES:
.SUFFIXES: .o .c .h

extern.h: global.h
	sed '/^[cCdils]/s/^/extern /' global.h > extern.h

.c.o:
	${CC} -c $*.c

${OBJECTS}:	Makefile	bump_codes.h	extern.h	header.h

clean:
	-@rm -f ${OBJECTS} > /dev/null 2>&1
