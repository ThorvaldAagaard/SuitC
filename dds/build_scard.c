//  name:
//      build_scard
//
//  purpose:
//      build_scard builds an array of SCARD sequences
//
//  parameters:
//      d               in      the depth in the NODE tree
//      s               in      the suit that is analysed

//  #include "header.h"

void build_scard( int d, int s )
{
    int           i, p, sh[20], shcnt = 0, t = st[d].t;
    COLOR         c;

    //  create a new SCARD entry
    scount++;
    ncalls[1]++;
    scard[t][s] = nextscard;
    if ( scountmax < scount ) {
        scountmax++;
        nextscard++;
    }
    else
        nextscard = nextscard->r;
    scard[t][s]->r = NIL;
    scard[t][s]->s = s;
    for ( p = 0; p < 4; p++ ) {
        scard[t][s]->brd[p] = brd[t-1][s][p];
        brd[t][s][p] = scard[t][s]->brd[p];
    }

    //  build the shifted card holding of each player
    c = brd[t-1][s][0] | brd[t-1][s][1] | brd[t-1][s][2] | brd[t-1][s][3];
    for ( i = ace; i >= two; i-- ) {
        if ( ! ( c & set(i) ) )
            shcnt++;
        sh[i] = shcnt;
    }
    for ( p = 0; p < 4; p++ ) {
        scard[t][s]->brd[p] = RENONCE;
        for ( i = ace; i >= two; i-- )
            if ( brd[t-1][s][p] & set(i) )
                scard[t][s]->brd[p] |= set(i) << sh[i];
        brd[t][s][p] = scard[t][s]->brd[p];
    }
}
