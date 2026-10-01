//  name:
//      join_scard
//
//  purpose:
//      join_scard joins adjacent SEQC card sequences of the same player
//
//  parameters:
//      brd             in      the cards of the 4 players
//
//  Copyright 2026, Gerrit A. van der Velde, Leiden

#include "header.h"

struct SCARD *join_scard( COLOR *brd )
{
    int           match, p, qcountback;
    unsigned long inx;
    struct JOINC  *ljoinc;
    struct SCARD  *ltable, *tscard;
    struct SEQC   *lseqc, *nextseqcback, *tseqc, zseqc;

    //  locate an existing JOINC slot
    inx = 0L;
    for ( p = 0; p < 4; p++ )
        inx = ( ( inx + (unsigned long) brd[p] ) * IA + IC ) % IM;
    inx = inx % (2*NJOINC);

    //  check the old JOINC slots
    ljoinc = joinctable[inx];
    while ( ljoinc ) {
        match = YES;
        for ( p = 0; p < 4; p++ )
            match &= brd[p] == ljoinc->brd[p];
        if ( match )
            return( ljoinc->scard );
        ljoinc = ljoinc->r;
    }

    //  create a new JOINC slot
    jcount++;
    if ( jcount == NJOINC ) {
        printf( "ERROR: No more memory available in the JOINC hash table; increase NJOINC\n" );
        print_stat( 'P' ); \
    }
    ljoinc = nextjoinc;
    ljoinc->r = joinctable[inx];
    joinctable[inx] = ljoinc;
    nextjoinc++;
    for ( p = 0; p < 4; p++ )
        ljoinc->brd[p] = brd[p];

    //  build the card sequences
    nextseqcback = nextseqc;
    qcountback = qcount;
    zseqc.d = build_seqc( brd );

    //  locate an existing SCARD slot
    inx = 0L;
    lseqc = zseqc.d;
    while( lseqc ) {
        inx = ( ( inx + (unsigned long) lseqc->n ) * IA + IC ) % IM;
        inx = ( ( inx + (unsigned long) lseqc->p ) * IA + IC ) % IM;
        lseqc = lseqc->d;
    }
    inx = inx % (2*NSCARD);

    //  check the old SCARD slots
    ltable = scardtable[inx];
    while ( ltable ) {

        lseqc = zseqc.d;
        tseqc = ltable->seqc;
        match = YES;
        while ( match && lseqc && tseqc ) {
            match &= lseqc->n == tseqc->n && lseqc->p == tseqc->p;
            lseqc = lseqc->d;
            tseqc = tseqc->d;
        }

        //  we found an old SCARD slot
        if ( match && lseqc == NIL && tseqc == NIL ) {
            ljoinc->scard = ltable;
            nextseqc = nextseqcback;
            qcount = qcountback;
            return( ltable );
        }

        ltable = ltable->r;
    }

    //  create a new SCARD entry
    scount++;
    if ( scount == NSCARD ) {
        printf( "No more memory for the SCARD entries; increase the value of NSCARD\n" );
        print_stat( 'P' );
    }
    tscard = nextscard;
    tscard->r = scardtable[inx];
    scardtable[inx] = tscard;
    nextscard++;
    tscard->seqc = zseqc.d;
    for ( p = 0; p < 4; p++ )
        tscard->brd[p] = brd[p];
    ljoinc->scard = tscard;

    return( tscard );
}
