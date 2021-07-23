//  name:
//      build_trick
//
//  purpose:
//      build_trick builds the NODE tree for a ply
//
//  parameters:
//      d               in      the depth in the tree
//      p               in      the player of this ply

#include "header.h"

void build_trick( int d, int p )
{
    int           lds, s, t = st[d].t;
    struct NODE   *anode, *bnode, *nnode, znode;
    struct SCARD  *lc;

    //  initialise
    lnode[d] = &znode;
    znode.r = NIL;

    for ( s = 0; s < 4; s++ ) {

        lds = st[d].ld ? s : lnode[st[d].l]->s;
        if ( brd[t][s][p] && ( st[d].ld || s == lds || ! brd[t][lds][p] ) ) {
            lc = sc[t][s];
            while ( lc ) {
                if ( lc->p == p )
                    build_node( d, s, p, lc->c );
                lc = lc->d;
            }
        }
    }

    //  order the plays
    anode = &znode;
    lnode[d] = anode->r;
next_node:
    while ( lnode[d]->r ) {

        bnode = lnode[d];
        nnode = bnode->r;
        while ( nnode ) {

            if ( ( lnode[d]->qt  < nnode->qt ) ||
                 ( lnode[d]->qt == nnode->qt && lnode[d]->s == nnode->s && lnode[d]->c  > nnode->c ) ||
                 ( lnode[d]->qt == nnode->qt && lnode[d]->s == trump    && nnode->s    != trump    ) ) {
                bnode->r = nnode->r;
                anode->r = nnode;
                nnode->r = lnode[d];
                lnode[d] = nnode;
                goto next_node;
            }

            bnode = nnode;
            nnode = bnode->r;
        }

        anode = lnode[d];
        lnode[d] = anode->r;
    }

    lnode[d] = znode.r;
    lnode[d-1]->d = znode.r;
}
