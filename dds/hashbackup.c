//  name:
//      hashbackup
//
//  purpose:
//      hashbackup determines the index of a BACKUP entry in the hash table
//      the routine returns YES if an existing entry is found in
//      backuptable and NO otherwise.
//
//  parameters:
//      d               in      the depth in the NODE tree
//      b               in      the board that is analysed
//
//  Copyright 2025, Gerrit A. van der Velde, Leiden

#include "header.h"

int hashbackup( int d, int b )
{
    int           match, p, s, t = st[d].t;
    unsigned long inx;

    //  create the JOINC slots
    ncalls[1]++;
    for ( s = 0; s < 4; s++ ) {
        if ( brd[t-1][b][s][west] | brd[t-1][b][s][north] | brd[t-1][b][s][east] | brd[t-1][b][s][south] )
            join_scard( d, b, s );
        else {
            scard[t][b][s] = NIL;
            for ( p = 0; p < 4; p++ )
                brd[t][b][s][p] = RENONCE;
        }
    }

    //  locate an existing BACKUP slot
    inx = ( (unsigned long) wnode[d-1]->w_p * IA + IC ) % IM;
    inx = ( ( inx + (unsigned long) trump ) * IA + IC ) % IM;
    for ( s = 0; s < 4; s++ )
        inx = ( ( inx + (unsigned long) scard[t][b][s] ) * IA + IC ) % IM;
    inx = inx % (2*NBACKUP);

    //  check the old BACKUP slots
    backup[t-1] = backuptable[inx];
    while ( backup[t-1] ) {
        match = wnode[d-1]->w_p == backup[t-1]->ld && trump == backup[t-1]->trump;
        for ( s = 0; s < 4; s++ )
            match &= scard[t][b][s] == backup[t-1]->scard[s];
        if ( match ) {
            wnode[d-1]->nh = backup[t-1]->nh;
            wnode[d-1]->nl = backup[t-1]->nl;
            wnode[d-1]->d = backup[t-1]->node;
            wnode[d-1]->t = backup[t-1]->t;
            return( YES );
        }
        backup[t-1] = backup[t-1]->r;
    }

    //  create a new BACKUP slot
    bcount++;
    backup[t-1] = nextbackup;
    nextbackup++;
    backup[t-1]->r = backuptable[inx];
    backuptable[inx] = backup[t-1];
    backup[t-1]->ld = wnode[d-1]->w_p;
    backup[t-1]->trump = trump;
    for ( s = 0; s < 4; s++ )
        backup[t-1]->scard[s] = scard[t][b][s];

    //  check the fill factor of the hash table
    if ( bcount == NBACKUP ) {
        printf( "ERROR: No more memory available in the BACKUP hash table; increase NBACKUP\n" );
        print_stat( 'P' ); \
    }

    return( NO );
}
