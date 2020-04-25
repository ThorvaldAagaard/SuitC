//  name:
//      create_ply3
//
//  purpose:
//      create_ply3 handles the 3rd hand play
//
//  parameters:
//      d               in      the depth in the tree
//      p               in      the player of this ply
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void create_ply3( int d, int p )
{
    int          i, j, n, q, t = st[d].t;
    COLOR        c, lwinc;
    struct NODE  znode;
    struct SCARD *sc;

    //  use the in initial card sequences
    if ( keepfulltree || throwin || defence || EW(p) )
        sc = &scards[0][p];

    //  use the card sequences for the current trick
    else {

        //  delete the small cards of the other players
        lwinc = MAX( MAX( low_c(wnode[d-2]->c), low_c(wnode[d-1]->c) ), low_c(brd[t][(p+1)%4]) );
        c = brd[t][(p+1)%4] | brd[t][(p+2)%4] | brd[t][(p+3)%4];
        for ( i = two; i <= ace; i++ )
            if ( addcard(i) < lwinc && addcard(i) & c )
                c ^= addcard(i) & c;

        //  build the card sequences for this trick
        q = NO;
        sc = &scards[t][p]; sc->n = 0;
        for ( i = 2; i <= ace; i++ ) {
            if ( brd[t][p] & addcard(i) ) {
                if ( q )
                    sc->c[sc->n] |= addcard(i);
                else {
                    q = YES;
                    sc->c[++sc->n] = addcard(i);
                }
            }
            else if ( c & addcard(i) )
                q = NO;
        }
    }

    wnode[d] = &znode;
    znode.r = NIL;
    for ( i = sc->n; i >= 0;i-- ) {
        st[d].c = sc->c[i];
        st[d].w = st[d-1].winc > sc->c[i] ? st[d-1].w : p;
        st[d].winc = st[d-1].winc > sc->c[i] ? st[d-1].winc : sc->c[i];

        //  create a new node
        if ( ! ( add_node( d, p, st[d].c ) ) )
            continue;

        //  handle the 4th hand play
        create_ply4( d+1, (p+1)%4 );
    }

    //  check the plays
    znode.r->l = NIL;
    wnode[d-1]->d = znode.r;
    ( NS( znode.r->p ) ? check_ns_plays( d, wnode ) : check_ew_plays( d, wnode ) );
}
