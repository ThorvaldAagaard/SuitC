//  name:
//      build_scard
//
//  purpose:
//      build_scard builds an array of SCARD sequences
//
//  parameters:
//      d               in      the depth in the NODE tree
//      s               in      the suit that is analysed
//
//  Copyright 2022, Gerrit A. van der Velde, Leiden

//  #include "header.h"

void build_scard( int d, int s )
{
    int           i, n = -1, p, q = unknown, t = st[d].t;
    struct SEQC   *sq;

    //  create a new SCARD entry
    scount++;
    ncalls[1]++;
    scard[t][s] = nextscard;
    if ( scountmax < scount ) {
        scountmax++;
        nextscard++;
        if ( scountmax == NSCARD ) {
            printf( "No more memory for the SCARD entries; increase the value of NSCARD\n" );
            exit( 99 );
        }
    }
    else
        nextscard = nextscard->r;
    scard[t][s]->r = NIL;
    scard[t][s]->s = s;
    for ( p = 0; p < 4; p++ ) {
        scard[t][s]->brd[p] = brd[t-1][s][p];
        brd[t][s][p] = scard[t][s]->brd[p];
    }

    //  build the card sequences
    sq = scard[t][s]->sq;
    for ( i = ace; i >= two; i-- ) {
        for ( p = 0; p < 4; p++ ) {
            if ( scard[t][s]->brd[p] & set(i) ) {
                if ( p != q ) {
                    q = p;
                    n++;
                    sq[n].n = 1;
                    sq[n].p = p;
                    sq[n].c = set(i);
                }
                else {
                    sq[n].n++;
                    sq[n].c |= set(i);
                }
            }
        }
    }
    scard[t][s]->n = ++n;
}
