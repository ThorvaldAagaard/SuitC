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
    int           bqcount = qcount, match, p, t = st[d].t;
    unsigned long inx;
    struct JOINC  *ljoinc;
    struct SCARD  *ltable;
    struct SEQC   *lseqc, *tseqc, zseqc;

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
            scard[t][b][s] = ljoinc->scard;
            for ( p = 0; p < 4; p++ )
                brd[t][b][s][p] = scard[t][b][s]->brd[p];
            return;
        }
        ljoinc = ljoinc->r;
    }

    //  create a new JOINC slot
    jcount++;
    if ( jcount == NJOINC ) {
        printf( "ERROR: No more memory available in the JOINC hash table; increase NJOINC\n" );
        print_stat( 'P' ); \
    }
    ljoinc = nextjoinc;
    nextjoinc++;
    ljoinc->r = joinctable[inx];
    joinctable[inx] = ljoinc;
    ljoinc->s = s;
    for ( p = 0; p < 4; p++ )
        ljoinc->brd[p] = brd[t-1][b][s][p];

    //  build the card sequences
    zseqc.r = build_seqc( d, b, s );

    //  locate an existing SCARD slot
    lseqc = zseqc.r;
    inx = ( (unsigned long) s * IA + IC ) % IM;
    while( lseqc ) {
        inx = ( ( inx + (unsigned long) lseqc->n ) * IA + IC ) % IM;
        inx = ( ( inx + (unsigned long) lseqc->p ) * IA + IC ) % IM;
        lseqc = lseqc->r;
    }
    inx = inx % (2*NSCARD);

    //  check the old SCARD slots
    ltable = scardtable[inx];
    while ( ltable ) {

        lseqc = zseqc.r;
        tseqc = ltable->seqc;
        match = s == ltable->s;
        while ( match && lseqc && tseqc ) {
            match &= lseqc->n == tseqc->n;
            match &= lseqc->p == tseqc->p;
            lseqc = lseqc->r;
            tseqc = tseqc->r;
        }

        //  we found an old SCARD slot
        if ( match && lseqc == NIL && tseqc == NIL ) {
            scard[t][b][s] = ltable;
            nextseqc = zseqc.r;
            qcount = bqcount;
            for ( p = 0; p < 4; p++ )
                brd[t][b][s][p] = ltable->brd[p];
            ljoinc->scard = ltable;
            return;
        }

        ltable = ltable->r;
    }

    //  create a new SCARD entry
    scount++;
    if ( scount == NSCARD ) {
        printf( "No more memory for the SCARD entries; increase the value of NSCARD\n" );
        print_stat( 'P' );
    }
    scard[t][b][s] = nextscard;
    nextscard++;
    scard[t][b][s]->r = scardtable[inx];
    scardtable[inx] = scard[t][b][s];
    scard[t][b][s]->s = s;
    scard[t][b][s]->seqc = zseqc.r;
    for ( p = 0; p < 4; p++ ) {
        scard[t][b][s]->brd[p] = brd[t-1][b][s][p];
        brd[t][b][s][p] = scard[t][b][s]->brd[p];
    }
    ljoinc->scard = scard[t][b][s];
}
