//  name:
//      create_ply2
//
//  purpose:
//      create_ply2 handles the 2nd hand play
//
//  parameters:
//      d               in      the depth in the tree
//      p               in      the player of this ply
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void create_ply2( int d, int p )
{
    int          i, n;
    struct NODE  znode;
    struct SCARD *sc;

    wnode[d] = &znode;
    znode.r = NIL;
    sc = &scards[0][p];
    for ( i = sc->n; i >= 0; i-- ) {
        st[d].c = sc->c[i];
        st[d].w = st[d-1].winc > sc->c[i] ? st[d-1].w : p;
        st[d].winc = st[d-1].winc > sc->c[i] ? st[d-1].winc : sc->c[i];

        //  create a new node
        if ( ! ( add_node( d, p, st[d].c ) ) )
            continue;

        //  handle the 3rd hand play
        create_ply3( d+1, (p+1)%4 );
    }

    //  check the plays
    znode.r->l = NIL;
    wnode[d-1]->d = znode.r;
    ( NS( znode.r->p ) ? check_ns_plays( d, wnode ) : check_ew_plays( d, wnode ) );
}
