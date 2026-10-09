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
    struct SEQC *nextsq, seqc[14], *tseqc, zseqc;

    //  build the SEQC card sequences
    nextsq = &seqc[0];
    tseqc = &zseqc;
    tseqc->d = NIL;
    for ( i = ace; i >= two; i-- ) {
        for ( p = 0; p < 4; p++ ) {
            if ( brd[p] & set(i) ) {
                if ( p != q ) {
                    q = p;
                    tseqc->d = nextsq;
                    tseqc = tseqc->d;
                    nextsq++;
                    tseqc->d = NIL;
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

    //  check the SEQC hashtable for existing SEQC lists
    return ( hashseqc( zseqc.d ) );
}
