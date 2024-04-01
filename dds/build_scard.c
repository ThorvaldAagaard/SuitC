//  name:
//      build_scard
//
//  purpose:
//      build_scard builds an array of SCARD sequences
//
//  parameters:
//      d               in      the depth in the NODE tree
//      b               in      the board that is analysed
//      s               in      the suit that is analysed
//
//  Copyright 2024, Gerrit A. van der Velde, Leiden

#include "header.h"

void build_scard( int d, int b, int s )
{
    int           i, p, q = unknown, t = st[d].t;
    struct SEQC   *tseqc, zseqc;

    //  create a new SCARD entry
    bump_scard( scard[b][t][s] );
    scard[b][t][s]->s = s;
    for ( p = 0; p < 4; p++ ) {
        scard[b][t][s]->brd[p] = brd[b][t-1][s][p];
        brd[b][t][s][p] = scard[b][t][s]->brd[p];
    }

    //  build the card sequences
    tseqc = &zseqc;
    tseqc->r = NIL;
    for ( i = ace; i >= two; i-- ) {
        for ( p = 0; p < 4; p++ ) {
            if ( scard[b][t][s]->brd[p] & set(i) ) {
                if ( p != q ) {
                    q = p;
                    bump_seqc( tseqc->r );
                    tseqc = tseqc->r;
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
    scard[b][t][s]->sq = zseqc.r;
}
