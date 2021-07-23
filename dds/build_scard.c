//  name:
//      build_scard
//
//  purpose:
//      build_scard builds an array of SCARD sequences
//
//  parameters:
//      s               in      the suit that is analysed
//      brd             in      the cards of the players

//  #include "header.h"

struct SCARD *build_scard( int s, COLOR *brd )
{
    int           i, n = -1, p, q = unknown;
    struct SCARD  *lscard;
    struct SEQC   *sq;

    //  create a new SCARD entry
    scount++;
    ncalls[1]++;
    lscard = nextscard;
    if ( scountmax < scount ) {
        scountmax++;
        nextscard++;
    }
    else
        nextscard = nextscard->r;
    lscard->r = NIL;
    lscard->s = s;
    for ( p = 0; p < 4; p++ )
        lscard->brd[p] = brd[p];

    //  build the card sequences
    sq = lscard->sq;
    for ( i = ace; i >= two; i-- ) {
        for ( p = 0; p < 4; p++ ) {
            if ( brd[p] & set(i) ) {
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

    lscard->n = ++n;
    return( lscard );
}
