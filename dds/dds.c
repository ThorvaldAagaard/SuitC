//  name:
//      dds
//
//  purpose:
//      dds is the main program for the double dummy solver
//
//  to produce a list of options type:
//      dds -h
//
//  Copyright 2025, Gerrit A. van der Velde, Leiden

#include "header.h"
#include "global.h"

int main( int argc, char **argv )
{
    int           d, inx, l;
    double        times[2];
    struct NODE   znode;

    //  initialise variables
    times[0] = TICK * clock();
    target.set = NO;
    for ( d = 0; d < 16; d++ )
        ncalls[d] = 0;
    for ( d = 0; d <= NPLIES; d++ ) {
        st[d].l = 4 * ( ( d - 1 ) / 4 ) + 1;
        st[d].la = ( d & 3 ) == 0;
        st[d].ld = ( d & 3 ) == 1;
        st[d].n = 0;
        st[d].t = ( d + 3 ) / 4;
    }

    //  allocate memory for the BACKUP entries
    bcount = 0;
    nextbackup = malloc( (NBACKUP+2) * sizeof( struct BACKUP ) );
    if ( nextbackup == NIL ) {
        printf( "The allocation of memory for the BACKUP entries failed\n" );
        print_stat( 'p' );
    }

    //  allocate memory for the JOINC entries
    jcount = 0;
    nextjoinc = malloc( (NJOINC+2) * sizeof( struct JOINC ) );
    if ( nextjoinc == NIL ) {
        printf( "The allocation of memory for the JOINC entries failed\n" );
        print_stat( 'p' );
    }

    //  allocate memory for the NODE tree
    //  initialise the NODE tree
    ncount = 0;
    ncountmax = 0;
    wnode[0] = &znode;
    wnode[0]->r = NIL;
    nextnode = malloc( (NNODE+2) * sizeof( struct NODE ) );
    if ( nextnode == NIL ) {
        printf( "The allocation of memory for the NODE tree failed\n" );
        print_stat( 'p' );
    }

    //  allocate memory for the SCARD entries
    scount = 0;
    nextscard = malloc( (NSCARD+2) * sizeof( struct SCARD ) );
    if ( nextscard == NIL ) {
        printf( "The allocation of memory for the SCARD entries failed\n" );
        print_stat( 'p' );
    }

    //  allocate memory for the SEQC entries
    qcount = 0;
    nextseqc = malloc( (NSEQC+2) * sizeof( struct SEQC ) );
    if ( nextseqc == NIL ) {
        printf( "The allocation of memory for the SEQC entries failed\n" );
        print_stat( 'p' );
    }

    //  process the command line
    ld = unknown;
    trump = discard;
    process_args( argc, argv );

    //  check the input and build the initial SCARD trees
    check_board();

    //  build the NODE tree and determine the optimal result for north-south
    boardcount[1] = boardcount[0];
    for ( l = 0; l < boardcount[0]; l++ )
        blist[1][l] = blist[0][l];
    if ( ld == unknown || trump == discard )
        play_board_start();
    play_board( 1, ld );
}
