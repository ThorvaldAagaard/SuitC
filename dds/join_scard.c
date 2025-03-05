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
    inx = ( (unsigned long) s * IA + IC ) % IM;
    for ( p = 0; p < 4; p++ )
        inx = ( ( inx + (unsigned long) brd[t-1][b][s][p] ) * IA + IC ) % IM;
    inx = inx % JHASHL;

    //  check the old JOINC slots
    ncalls[9]++;
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
    jfill++;
    ljoinc = nextjoinc;
    nextjoinc++;
    ljoinc->r = joinctable[inx];
    joinctable[inx] = ljoinc;
    ljoinc->s = s;
    for ( p = 0; p < 4; p++ )
        ljoinc->brd[p] = brd[t-1][b][s][p];

    //  convert the cards into a SCARD list
    build_scard( d, b, s );

    //  locate an existing SCARD slot
    lseqc = scard[t][b][s]->sq;
    inx = ( (unsigned long) scard[t][b][s]->s * IA + IC ) % IM;
    while( lseqc ) {
        inx = ( ( inx + (unsigned long) lseqc->n ) * IA + IC ) % IM;
        inx = ( ( inx + (unsigned long) lseqc->p ) * IA + IC ) % IM;
        lseqc = lseqc->r;
    }
    inx = inx % SHASHL;

    //  check the old SCARD slots
    ltable = scardtable[inx];
    while ( ltable ) {

        lseqc = scard[t][b][s]->sq;
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
                scard[t][b][s]->r = nextscard;
                nextscard = scard[t][b][s];
                scard[t][b][s] = ltable;
                lseqc = nextscard->sq;
                while ( lseqc ) { sqcount--; tseqc = lseqc->r; lseqc->r = nextseqc; nextseqc = lseqc; lseqc = tseqc; }
                ljoinc->sc = ltable;
                for ( p = 0; p < 4; p++ )
                    brd[t][b][s][p] = ltable->brd[p];
                return;
            }

        ltable = ltable->r;
    }

    //  insert the SCARD entry in the hash tables
    sfill++;
    scard[t][b][s]->r = scardtable[inx];
    scardtable[inx] = scard[t][b][s];
    ljoinc->sc = scard[t][b][s];

    //  check the fill factor of the hash table
    if ( 2 * sfill >= SHASHL ) {
        printf( "ERROR: No more memory available: shashl=%ld sfill=%d\n", SHASHL, sfill );
        print_stat( 'P' ); \
    }

    //  check the fill factor of the hash table
    if ( 2 * jfill >= JHASHL ) {
        printf( "ERROR: No more memory available: jhashl=%ld jfill=%d\n", JHASHL, jfill );
        print_stat( 'P' ); \
    }
}
