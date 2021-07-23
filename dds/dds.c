//  name:
//      dds
//
//  purpose:
//      dds is the main program for the double dummy solver
//
//  to produce a list of options type:
//      dds -h

#include "header.h"

int main( int argc, char **argv )
{
    int           d, inx, p, s;
    double        times[2];
    struct NODE   znode;

    //  initialise variables
    times[0] = TICK * clock();
    bfill = 0;
    jfill = 0;
    ncount = 0;
    ncountmax = 0;
    scount = 0;
    scountmax = 0;
    sfill = 0;
    target.set = NO;
    for ( s = 0; s < 5; s++ )
        for ( p = 0; p < 4; p++ )
            brd[0][s][p] = RENONCE;
    for ( d = 0; d < 5; d++ )
        ncalls[d] = 0;
    for ( d = 0; d <= 52; d++ ) {
        st[d].l = 4 * ( ( d - 1 ) / 4 ) + 1;
        st[d].la = ( d & 3 ) == 0;
        st[d].ld = ( d & 3 ) == 1;
        st[d].n = 0;
        st[d].t = ( d + 3 ) / 4;
    }
    for ( inx = 0; inx < BHASHL+100; inx++ )
        backuptable[inx] = NIL;
    for ( inx = 0; inx < JHASHL+100; inx++ )
        joinctable[inx] = NIL;
    for ( inx = 0; inx < SHASHL+100; inx++ )
        scardtable[inx] = NIL;

    //  initialise the BACKUP tree
    nextbackup = malloc( BHASHL * sizeof( struct BACKUP ) );

    //  initialise the JOINC tree
    nextjoinc = malloc( JHASHL * sizeof( struct JOINC ) );

    //  initialise the NODE tree
    lnode[0] = &znode;
    lnode[0]->r = NIL;
    nextnode = malloc( 600 * 1048576L * sizeof( struct NODE ) );

    //  initialise the SCARD tree
    nextscard = malloc( 1048576L * sizeof( struct SCARD ) );

    //  process the command line
    ld = unknown;
    trump = discard;
    process_args( argc, argv );

    //  check the input and build the initial SCARD trees
    check_board();

    //  build the NODE tree and determine the optimal result for north-south
    if ( ld == unknown || trump == discard )
        play_board_start();
    else
        hashbackup( 1 );
    play_board( 1, ld );
}
