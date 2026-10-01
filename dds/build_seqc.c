//  name:
//      build_seqc
//
//  purpose:
//      build_seqc builds the list of SEQC card sequences
//
//  parameters:
//      brd             in      the SEQC card sequences of the 4 players
//
//  Copyright 2026, Gerrit A. van der Velde, Leiden

#include "header.h"

struct SEQC *build_seqc( COLOR *brd )
{
    int         i, p, q = unknown;
    struct SEQC *tseqc, zseqc;

    //  build the SEQC card sequences
    tseqc = &zseqc;
    tseqc->d = NIL;
    for ( i = ace; i >= two; i-- ) {
        for ( p = 0; p < 4; p++ ) {
            if ( brd[p] & set(i) ) {
                if ( p != q ) {
                    q = p;
                    qcount++;
                    if ( qcount == NSEQC ) {
                        printf( "No more memory for the SEQC entries; increase the value of NSEQC\n" );
                        print_stat( 'P' );
                    }
                    tseqc->d = nextseqc;
                    tseqc = tseqc->d;
                    nextseqc++;
                    tseqc->d = NIL;
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

    return( zseqc.d );
}
