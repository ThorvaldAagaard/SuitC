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
//  Copyright 2024, Gerrit A. van der Velde, Leiden

#include "header.h"

void join_scard( int d, int b, int s )
{
    int           match, n, p, t = st[d].t;
    unsigned long inx;
    struct JOINC  *ljoinc;
    struct SCARD  *ltable;
    struct SEQC   *lseqc, *tseqc;

    //  locate an existing JOINC slot
    inx = ( (unsigned long) s * IA + IC ) % IM;
    for ( p = 0; p < 4; p++ )
        inx = ( ( inx + (unsigned long) brd[b][t-1][s][p] ) * IA + IC ) % IM;
    inx = inx % JHASHL;

    //  check the old JOINC slots
    ncalls[9]++;
    ljoinc = joinctable[inx];
    while ( ljoinc ) {
        match = s == ljoinc->s;
        for ( p = 0; p < 4; p++ )
            match &= brd[b][t-1][s][p] == ljoinc->brd[p];
        if ( match ) {
            scard[b][t][s] = ljoinc->sc;
            for ( p = 0; p < 4; p++ )
                brd[b][t][s][p] = scard[b][t][s]->brd[p];
            return;
        }
        ljoinc = ljoinc->r;
    }

    //  create a new JOINC slot
    jfill++;
    ljoinc = nextjoinc;
    nextjoinc++;
    ljoinc->r = joinctable[inx];
    joinctable[inx] = ljoinc;
    ljoinc->s = s;
    for ( p = 0; p < 4; p++ )
        ljoinc->brd[p] = brd[b][t-1][s][p];

    //  convert the cards into a SCARD list
    build_scard( d, b, s );

    //  locate an existing SCARD slot
    lseqc = scard[b][t][s]->sq;
    inx = ( (unsigned long) scard[b][t][s]->s * IA + IC ) % IM;
    while( lseqc ) {
        inx = ( ( inx + (unsigned long) lseqc->n ) * IA + IC ) % IM;
        inx = ( ( inx + (unsigned long) lseqc->p ) * IA + IC ) % IM;
        lseqc = lseqc->r;
    }
    inx = inx % SHASHL;

    //  check the old SCARD slots
    ltable = scardtable[inx];
    while ( ltable ) {

        lseqc = scard[b][t][s]->sq;
        tseqc = ltable->sq;
        match = YES;

            while ( match && lseqc && tseqc ) {
                match &= lseqc->n == tseqc->n;
                match &= lseqc->p == tseqc->p;
                lseqc = lseqc->r;
                tseqc = tseqc->r;
            }

            //  we found an old SCARD slot
            if ( match && lseqc == NIL && tseqc == NIL ) {
                scount--;
                scard[b][t][s]->r = nextscard;
                nextscard = scard[b][t][s];
                scard[b][t][s] = ltable;
                ljoinc->sc = ltable;
                for ( p = 0; p < 4; p++ )
                    brd[b][t][s][p] = ltable->brd[p];
                return;
            }

        ltable = ltable->r;
    }

    //  insert the SCARD entry in the hash tables
    sfill++;
    scard[b][t][s]->r = scardtable[inx];
    scardtable[inx] = scard[b][t][s];
    ljoinc->sc = scard[b][t][s];

    //  check the fill factor of the hash table
    if ( 2 * sfill >= SHASHL ) {
        printf( "ERROR: No more memory available: shashl=%d sfill=%d\n", SHASHL, sfill );
        exit( 99 );
    }

    //  check the fill factor of the hash table
    if ( 2 * jfill >= JHASHL ) {
        printf( "ERROR: No more memory available: jhashl=%d jfill=%d\n", JHASHL, jfill );
        exit( 99 );
    }
}
