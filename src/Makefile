#-----------------------------------------------------------------------
#
#  Makefile for program SuitC
#
#  This is a restart from ExpertPlay Rev6.102
#
#  Copyright 2020, Gerrit A. van der Velde, Leiden
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

OBJECTS = 				\
	  add_node.o			\
	  add_quirkd.o			\
	  build_selected_tree.o		\
	  check_ew_plays.o		\
	  check_ns_plays.o		\
	  check_quirkd.o		\
	  check_zermelo.o		\
	  clean_quirkd.o		\
	  convert_tree_to_quirk.o	\
	  copy_node.o			\
	  copy_tree.o			\
	  count_plays.o			\
	  count_quirkd.o		\
	  create_all_boards.o		\
	  create_board_header.o		\
	  create_boards.o		\
	  create_ply1.o			\
	  create_ply2.o			\
	  create_ply3.o			\
	  create_ply4.o			\
	  create_tricks.o		\
	  delete_board.o		\
	  delete_quirkd.o		\
	  delete_tree.o			\
	  determine_trick_count.o	\
	  evaluate_best_results.o	\
	  evaluate_mp_results.o		\
	  evaluate_mp_scores.o		\
	  fold_ew_tree.o		\
	  fold_ns_tree.o		\
	  fold_quirkd.o			\
	  limit_matchpoint_nodes.o	\
	  limit_quirkd.o		\
	  merge_nodes.o			\
	  merge_quirkd.o		\
	  merge_tree.o			\
	  minimax.o			\
	  print_all_results.o		\
	  print_boards.o		\
	  print_play_count.o		\
	  print_quirkd.o		\
	  print_results.o		\
	  print_summary_results.o	\
	  print_tree.o			\
	  process_args.o		\
	  select_ns_plays.o		\
	  sort_plays.o			\
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

${OBJECTS}:	Makefile	header.h

clean:
	-@rm -f ${OBJECTS} > /dev/null 2>&1
