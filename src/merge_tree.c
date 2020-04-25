//  name:
//      merge_tree
//
//  purpose:
//      merge_tree merges subtrees if they contain a common structure of plays
//
//  parameters:
//      d               in      the depth in the NODE tree
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void merge_tree( int d )
{
    struct NODE  *bnode[NPLIES+1], *lnode[NPLIES+1];

    bnode[d-1] = tnode[d-1];
    lnode[d-1] = tnode[d-1];
    bnode[d] = tnode[d-1]->d;
    while ( bnode[d]->r )
        bnode[d] = bnode[d]->r;

    if ( EW( bnode[d]->p ) ) {

    next_bnode:
        while ( bnode[d]->l ) {

            tnode[d] = bnode[d]->l;
            while ( tnode[d] ) {

                if ( tnode[d]->p == bnode[d]->p ) {

                    stm.bc = bnode[d]->c;
                    stm.lc = tnode[d]->c;

                    lnode[d] = copy_tree( d, tnode[d] );
                    lnode[d]->r = tnode[d]->r;
                    lnode[d]->r->l = lnode[d];
                    tnode[d]->r = lnode[d];
                    lnode[d]->l = tnode[d];

                    if ( ! bnode[d]->d || fold_tree( d+1, lnode, bnode ) ) {
                        merge_nodes( lnode[d], bnode[d] );
                        lnode[d] = bnode[d]->l;
                        delete_tree( d, bnode );
                        delete_tree( d, tnode );
                        bnode[d] = lnode[d];
                        goto next_bnode;
                    }

                    else
                        delete_tree( d, lnode );
                }

                tnode[d] = tnode[d]->l;
            }

            bnode[d] = bnode[d]->l;
        }
    }

    //  investigate the sons
    tnode[d] = tnode[d-1]->d;
    if ( tnode[d]->d ) {
        while ( tnode[d] ) {
            merge_tree( d+1 );
            tnode[d] = tnode[d]->r;
        }
    }

    //  merge the nodes if there are no sons
    else if ( EW(tnode[d]->p) ) {
        while ( tnode[d]->r ) {
            lnode[d] = tnode[d]->r;
            merge_nodes( tnode[d], lnode[d] );
            delete_tree( d, lnode );
        }
    }

    //  determine the winning cards
    tnode[d] = tnode[d-1]->d;
    tnode[d-1]->winc = (d-1)%4 ? RENONCE : tnode[d-1]->winc;
    while ( tnode[d] ) {
        if ( (d-1)%4 )
            tnode[d-1]->winc |= tnode[d]->winc;
        tnode[d] = tnode[d]->r;
    }
}
