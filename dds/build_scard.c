//  name:
//      build_scard
//
//  purpose:
//      build_scard builds and checks an SCARD entry
//
//  parameters:
//      d               in      the depth in the NODE tree
//      b               in      the board that is analysed
//      s               in      the suit that is analysed
//      lookup          in      check the SCARD hash table for an existing entry
//
//  Copyright 2025, Gerrit A. van der Velde, Leiden

#include "header.h"

void build_scard( int d, int b, int s, int lookup )
{
    int           bqcount = qcount, i, match, p, q = unknown, t = st[d].t;
    unsigned long inx;
    struct SCARD  *ltable;
    struct SEQC   *lseqc, *tseqc, zseqc;

    //  build the card sequences
    tseqc = &zseqc;
    tseqc->r = NIL;
    for ( i = ace; i >= two; i-- ) {
        for ( p = 0; p < 4; p++ ) {
            if ( brd[t-1][b][s][p] & set(i) ) {
                if ( p != q ) {
                    q = p;
                    qcount++;
                    if ( qcount == NSEQC ) {
                        printf( "No more memory for the SEQC entries; increase the value of NSEQC\n" );
                        print_stat( 'P' );
                    }
                    tseqc->r = nextseqc;
                    tseqc = tseqc->r;
                    tseqc->r = NIL;
                    nextseqc++;
                    tseqc->n = 1;
                    tseqc->p = p;
                    tseqc->c = set(i);
                }
                else {
                    tseqc->n++;
                    tseqc->c |= set(i);
                }
            }
        }
    }

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
    if ( lookup ) {

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
                return;
            }

            ltable = ltable->r;
        }
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
}
