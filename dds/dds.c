//  name:
//      dds
//
//  purpose:
//      dds is the main program for the double dummy solver
//
//  to produce a list of options type:
//      dds -h
//
//  Copyright 2022, Gerrit A. van der Velde, Leiden

#include "header.h"
#include "bottomup_tree.c"
#include "build_node.c"
#include "build_ply.c"
#include "build_scard.c"
#include "build_tree.c"
#include "check_board.c"
#include "delete_tree.c"
#include "hashbackup.c"
#include "join_scard.c"
#include "order_plays.c"
#include "play_board.c"
#include "play_board_start.c"
#include "print_board.c"
#include "print_node.c"
#include "print_scard.c"
#include "print_stat.c"
#include "print_tree.c"
#include "process_args.c"

int main( int argc, char **argv )
{
    int           d, inx, l;
    double        times[2];
    struct NODE   znode;

    //  initialise variables
    times[0] = TICK * clock();
    hlist[0].n = 0;
    target.set = NO;
    for ( d = 0; d < 5; d++ )
        ncalls[d] = 0;
    for ( d = 0; d <= 52; d++ ) {
        st[d].l = 4 * ( ( d - 1 ) / 4 ) + 1;
        st[d].la = ( d & 3 ) == 0;
        st[d].ld = ( d & 3 ) == 1;
        st[d].n = 0;
        st[d].t = ( d + 3 ) / 4;
    }

    //  initialise the BACKUP tree
    bfill = 0;
    nextbackup = malloc( BHASHL * sizeof( struct BACKUP ) );
    for ( inx = 0; inx < BHASHL+100; inx++ )
        backuptable[inx] = NIL;

    //  initialise the JOINC tree
    jfill = 0;
    nextjoinc = malloc( JHASHL * sizeof( struct JOINC ) );
    for ( inx = 0; inx < JHASHL+100; inx++ )
        joinctable[inx] = NIL;

    //  initialise the NODE tree
    ncount = 0;
    ncountmax = 0;
    lnode[0] = &znode;
    lnode[0]->r = NIL;
    nextnode = malloc( NNODE * sizeof( struct NODE ) );

    //  initialise the SCARD tree
    sfill = 0;
    scount = 0;
    scountmax = 0;
    nextscard = malloc( NSCARD * sizeof( struct SCARD ) );
    for ( inx = 0; inx < SHASHL+100; inx++ )
        scardtable[inx] = NIL;

    //  process the command line
    ld = unknown;
    trump = discard;
    process_args( argc, argv );

    //  check the input and build the initial SCARD trees
    check_board();

    //  build the NODE tree and determine the optimal result for north-south
    hlist[1].n = hlist[0].n;
    for ( l = 0; l < hlist[0].n; l++ )
        hlist[1].h[l] = hlist[0].h[l];
    if ( ld == unknown || trump == discard )
        play_board_start();
    play_board( 1, ld );
}
