#-----------------------------------------------------------------------
#
#  Makefile for program ExpertPlay
#
#  This is a restart from Rev6.102
#
#  Copyright 2019, Gerrit A. van der Velde, Leiden
#
#-----------------------------------------------------------------------

SHELL	= /bin/ksh
CC	= gcc -O3

EXEC	= ../bin/ExpertPlay
EXEC1	= ../bin/xp
EXEC2	= ../bin/1s
EXEC3	= ../bin/sp
EXEC4	= ../bin/sc
EXEC5	= ../bin/SuitC

OBJECTS = 					\
	  add_node.o				\
	  add_quirk.o				\
	  build_selected_tree.o			\
	  bump_index.o				\
	  bump_node.o				\
	  bump_quirk.o				\
	  check_ew_plays.o			\
	  check_ns_plays.o			\
	  check_quirk.o				\
	  check_zermelo.o			\
	  clean_quirk.o				\
	  clean_quirkh.o			\
	  convert_tree_to_quirk.o		\
	  copy_node.o				\
	  copy_tree.o				\
	  count_quirk.o				\
	  count_quirkh.o			\
	  create_all_boards.o			\
	  create_board_header.o			\
	  create_boards.o			\
	  create_ply1.o				\
	  create_ply2.o				\
	  create_ply3.o				\
	  create_ply4.o				\
	  create_tricks.o			\
	  delete_board.o			\
	  delete_quirk.o			\
	  delete_quirkh.o			\
	  delete_tree.o				\
	  determine_trick_count.o		\
	  evaluate_all_results_quirk.o		\
	  evaluate_all_results_quirkh.o		\
	  evaluate_mp_results.o			\
	  evaluate_mp_scores_quirk.o		\
	  evaluate_mp_scores_quirkh.o		\
	  evaluate_results_only_quirk.o		\
	  evaluate_results_only_quirkh.o	\
	  fold_quirk.o				\
	  fold_quirkh.o				\
	  fold_tree.o				\
	  limit_matchpoint_nodes.o		\
	  limit_quirk.o				\
	  limit_quirkh.o			\
	  merge_nodes.o				\
	  merge_quirk.o				\
	  merge_quirkh.o			\
	  merge_tree.o				\
	  minimax.o				\
	  print_all_results_quirk.o		\
	  print_all_results_quirkh.o		\
	  print_boards.o			\
	  print_results.o			\
	  print_results_only_quirk.o		\
	  print_results_only_quirkh.o		\
	  print_tree.o				\
	  process_args.o			\
	  select_ns_plays.o			\
	  suitc.o

${EXEC}: ${OBJECTS}
	${CC} -o ${EXEC} ${OBJECTS}
	[[ -f ${EXEC}.exe ]] && mv ${EXEC}.exe ${EXEC}
	rm -f ${EXEC1}; ln -s ${EXEC} ${EXEC1}
	rm -f ${EXEC2}; ln -s ${EXEC} ${EXEC2}
	rm -f ${EXEC3}; ln -s ${EXEC} ${EXEC3}
	rm -f ${EXEC4}; ln -s ${EXEC} ${EXEC4}
	rm -f ${EXEC5}; ln -s ${EXEC} ${EXEC5}

.SUFFIXES:
.SUFFIXES: .o .c .h

.c.o:
	${CC} -c $*.c

${OBJECTS}:			Makefile	header.h
convert_tree_to_quirk.o:	bump_quirkd.c	bump_quirkh.c
expertplay.o:			bump_quirkd.c	bump_quirkh.c
fold_quirkh.o:			add_quirkh.c	bump_quirkd.c	bump_quirkh.c	check_quirkh.c
merge_quirkh.o:			add_quirkh.c	bump_quirkd.c	bump_quirkh.c	check_quirkh.c

clean:
	-@rm -f ${OBJECTS} > /dev/null 2>&1
