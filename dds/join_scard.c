//  name:
//      join_scard
//
//  purpose:
//      join_scard joins adjacent card sequences of the same player
//
//  parameters:
//      d               in      the depth in the NODE tree
//      b               in      the board that is analysed
//      s               in      the suit that is analysed
//
//  Copyright 2025, Gerrit A. van der Velde, Leiden

#include "header.h"

void join_scard( int d, int b, int s )
{
    int           match, p, t = st[d].t;
    unsigned long inx;
    struct JOINC  *ljoinc;
    struct SCARD  *ltable;
    struct SEQC   *lseqc, *tseqc;

    //  locate an existing JOINC slot
    ncalls[2]++;
    inx = ( (unsigned long) s * IA + IC ) % IM;
    for ( p = 0; p < 4; p++ )
        inx = ( ( inx + (unsigned long) brd[t-1][b][s][p] ) * IA + IC ) % IM;
    inx = inx % (2*NJOINC);

    //  check the old JOINC slots
    ljoinc = joinctable[inx];
    while ( ljoinc ) {
        match = s == ljoinc->s;
        for ( p = 0; p < 4; p++ )
            match &= brd[t-1][b][s][p] == ljoinc->brd[p];
        if ( match ) {
            scard[t][b][s] = ljoinc->sc;
            for ( p = 0; p < 4; p++ )
                brd[t][b][s][p] = scard[t][b][s]->brd[p];
            return;
        }
        ljoinc = ljoinc->r;
    }

    //  create a new JOINC slot
    jcount++;
    ljoinc = nextjoinc;
    nextjoinc++;
    ljoinc->r = joinctable[inx];
    joinctable[inx] = ljoinc;
    ljoinc->s = s;
    for ( p = 0; p < 4; p++ )
        ljoinc->brd[p] = brd[t-1][b][s][p];
    build_scard( d, b, s, YES );
    ljoinc->sc = scard[t][b][s];

    //  check the fill factor of the hash table
    if ( jcount == NJOINC ) {
        printf( "ERROR: No more memory available in the JOINC hash table; increase NJOINC\n" );
        print_stat( 'P' ); \
    }
}
