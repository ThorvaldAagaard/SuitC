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
//  Copyright 2024, Gerrit A. van der Velde, Leiden

#include "header.h"

int hashbackup( int d, int b )
{
    int           match, p, s, t = st[d].t;
    unsigned long inx;
    struct BACKUP *lbackup;

    //  create the JOINC slots
    for ( s = 0; s < 4; s++ ) {
        if ( brd[b][t-1][s][0] | brd[b][t-1][s][1] | brd[b][t-1][s][2] | brd[b][t-1][s][3] )
            join_scard( d, b, s );
        else {
            scard[b][t][s] = NIL;
            for ( p = 0; p < 4; p++ )
                brd[b][t][s][p] = RENONCE;
        }
    }

    //  locate an existing BACKUP slot
    ncalls[3]++;
    inx = ( (unsigned long) wnode[d-1]->w_p * IA + IC ) % IM;
    inx = ( ( inx + (unsigned long) trump ) * IA + IC ) % IM;
    for ( s = 0; s < 4; s++ )
        inx = ( ( inx + (unsigned long) scard[b][t][s] ) * IA + IC ) % IM;
    inx = inx % BHASHL;

    //  check the old BACKUP slots
    lbackup = backuptable[inx];
    while ( lbackup ) {
        match = wnode[d-1]->w_p == lbackup->ld && trump == lbackup->trump;
        for ( s = 0; s < 4; s++ )
            match &= scard[b][t][s] == lbackup->sc[s];
        if ( match ) {
            backup[t-1] = lbackup;
            wnode[d-1]->nh = lbackup->nh;
            wnode[d-1]->nl = lbackup->nl;
            wnode[d-1]->d = lbackup->node;
            wnode[d-1]->t = lbackup->t;
            return( YES );
        }
        lbackup = lbackup->r;
    }

    //  build the NODE tree for the next trick
    build_ply( d, b, wnode[d-1]->w_p );

    //  create a new BACKUP slot
    bfill++;
    lbackup = nextbackup;
    nextbackup++;
    lbackup->r = backuptable[inx];
    backuptable[inx] = lbackup;
    backup[t-1] = lbackup;
    lbackup->ld = wnode[d-1]->w_p;
    lbackup->trump = trump;
    for ( s = 0; s < 4; s++ )
        lbackup->sc[s] = scard[b][t][s];

    //  check the fill factor of the hash table
    if ( ( 2 * bfill >= BHASHL ) ) {
        printf( "ERROR: No more memory available: bhashl=%d bfill=%d\n", BHASHL, bfill );
        exit( 99 );
    }

    return( NO );
}
