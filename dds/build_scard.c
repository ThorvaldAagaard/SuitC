//  name:
//      build_scard
//
//  purpose:
//      build_scard builds an array of card sequences
//
//  parameters:
//      d               in      the depth in the NODE tree
//      s               in      the suit that is analysed

//  #include "header.h"

struct SCARD *build_scard( int d, int s )
{
    int           i, n = -1, p, q = unknown, t = st[d].t;
    struct SCARD  *lscard;
    struct SEQC   *sq;

    bump_scard( lscard );
    lscard->s = s;
    for ( p = 0; p < 4; p++ ) {
        brd[t][s][p] = brd[t-1][s][p];
        lscard->brd[p] = brd[t-1][s][p];
    }

    sq = lscard->sq;
    for ( i = ace; i >= two; i-- ) {
        for ( p = 0; p < 4; p++ ) {
            if ( brd[t][s][p] & set(i) ) {
                if ( p != q ) {
                    n++;
                    q = p;
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
