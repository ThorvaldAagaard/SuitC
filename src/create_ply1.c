//  name:
//      create_ply1
//
//  purpose:
//      create_ply1 handles the lead
//
//  parameters:
//      d               in      the depth in the tree
//      lead[4]         in      flag to indicate valid leads
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void create_ply1( int d, int lead[4] )
{
    int          i, j, p, q, t = st[d].t;
    COLOR        c, lwinc;
    struct NODE  znode;
    struct SCARD *ic, *sc;

    wnode[d] = &znode;
    znode.r = NIL;
    for ( p = 0; p < 4; p++ ) {
        if ( lead[p] ) {

            //  adjust the number of free entries
            nfree[t][west] = nfree[t-1][west];
            nfree[t][east] = nfree[t-1][east];
            nfree[t][north] = nfree[t-1][north];
            nfree[t][south] = nfree[t-1][south];
            if ( ( t > 1 ) && ( w[t-1] != p ) )
                nfree[t][p] -= 1;

            //  use the in initial card sequences
            if ( keepfulltree || throwin || defence || EW(p) )
                sc = &scards[0][p];

            //  use the card sequences for the current trick
            else {

                //  delete the small cards of the other players
                lwinc = MAX( MAX( low_c(brd[t][(p+1)%4]), low_c(brd[t][(p+2)%4]) ), low_c(brd[t][(p+3)%4]) );
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

            for ( i = sc->n; i >= 0; i-- ) {
                st[d].c = sc->c[i];
                st[d].w = p;
                st[d].winc = sc->c[i];

                //  check for a specified openings lead
                if ( ! st[d].c || st[d].c == addcard(ruff) )
                    continue;
                if ( ( t == 1 ) && ( leader == p ) && ! ( st[d].c & opening_lead ) )
                    continue;

                //  create a new node
                if ( ! ( add_node( d, p, st[d].c ) ) )
                    continue;

                //  handle the 2nd hand play
                create_ply2( d+1, (p+1)%4 );
            }
        }
    }

    //  check the plays
    znode.l = NIL;
    wnode[d-1]->d = znode.r;
    if ( wnode[d-1]->d )
        ( NS( znode.r->p ) ? check_ns_plays( d, wnode ) : check_ew_plays( d, wnode ) );
}
