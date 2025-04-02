//  name:
//      build_seqc
//
//  purpose:
//      build_seqc builds the list of card sequences
//
//  parameters:
//      d               in      the depth in the NODE tree
//      b               in      the board that is analysed
//      s               in      the suit that is analysed
//
//  Copyright 2025, Gerrit A. van der Velde, Leiden

#include "header.h"

struct SEQC *build_seqc( int d, int b, int s )
{
    int           i, p, q = unknown, t = st[d].t;
    struct SEQC   *tseqc, zseqc;

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
                    nextseqc++;
                    tseqc = tseqc->r;
                    tseqc->r = NIL;
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

    return( zseqc.r );
}
