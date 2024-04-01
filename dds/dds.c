//  name:
//      dds
//
//  purpose:
//      dds is the main program for the double dummy solver
//
//  to produce a list of options type:
//      dds -h
//
//  Copyright 2024, Gerrit A. van der Velde, Leiden

#include "header.h"
#include "global.h"

int main( int argc, char **argv )
{
    int           d, inx, l;
    double        times[2];
    struct NODE   znode;

    //  initialise variables
    times[0] = TICK * clock();
    blist[0].n = 0;
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
    wnode[0] = &znode;
    wnode[0]->r = NIL;
    nextnode = malloc( NNODE * sizeof( struct NODE ) );

    //  initialise the SCARD tree
    sfill = 0;
    scount = 0;
    scountmax = 0;
    nextscard = malloc( NSCARD * sizeof( struct SCARD ) );
    for ( inx = 0; inx < SHASHL+100; inx++ )
        scardtable[inx] = NIL;

    //  initialise the SEQC list
    sqcount = 0;
    sqcountmax = 0;
    nextseqc = malloc( NSEQC * sizeof( struct SEQC ) );
    if ( nextseqc == NIL ) {
        printf( "The allocation of memory for the SEQC list failed\n" );
        exit( 99 );
    }

    //  process the command line
    ld = unknown;
    trump = discard;
    process_args( argc, argv );

    //  check the input and build the initial SCARD trees
    check_board();

    //  build the NODE tree and determine the optimal result for north-south
    blist[1].n = blist[0].n;
    for ( l = 0; l < blist[0].n; l++ )
        blist[1].b[l] = blist[0].b[l];
    if ( ld == unknown || trump == discard )
        play_board_start();
    play_board( 1, ld );
}
