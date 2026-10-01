//  name:
//      build_ply
//
//  purpose:
//      build_ply builds the NODE tree for a ply
//
//  parameters:
//      d               in      the depth in the NODE tree
//      b               in      the board that is analysed
//      p               in      the player of this ply
//
//  Copyright 2025, Gerrit A. van der Velde, Leiden

#include "header.h"

void build_ply( int d, int b, int p )
{
    int           lds, s, t = st[d].t;
    struct NODE   znode;
    struct SEQC   *lseqc;

    //  initialise
    wnode[d] = &znode;
    znode.r = NIL;

    for ( s = 0; s < 4; s++ ) {
        lds = st[d].ld ? s : wnode[st[d].l]->s;
        if ( brd[t][b][s][p] && ( st[d].ld || s == lds || ! brd[t][b][lds][p] ) ) {
            lseqc = scard[t][b][s]->seqc;
            while ( lseqc ) {
                if ( lseqc->p == p )
                    build_node( d, b, s, p, lseqc->c );
                lseqc = lseqc->d;
            }
        }
    }

    //  order the plays
    wnode[d] = znode.r;
    wnode[d-1]->d = znode.r;
    order_plays( d );
}
