//  name:
//      delete_tree
//
//  purpose:
//      delete_tree deletes a NODE tree
//
//  parameters:
//      d_start         in      the depth in the NODE tree
//      root            in      the root of the NODE tree to be deleted
//
//  Copyright 2025, Gerrit A. van der Velde, Leiden

#include "header.h"

struct NODE *delete_tree( int d_start, struct NODE *root )
{
    int           d = d_start;
    struct NODE   *anode, *bnode[53], *nnode;

    //  unlink root from the tree
    nnode = root->r;
    if ( wnode[d_start-1]->d == root ) {
        wnode[d_start-1]->d = root->r;
    }
    else {
        anode = wnode[d_start-1]->d;
        while ( anode->r != root )
            anode = anode->r;
        anode->r = root->r;
    }
    root->r = NIL;

    //  delete the tree
    bnode[d] = root;
    while ( bnode[d] ) {

        if ( bnode[d]->d && ! st[d].la )
            bnode[d+1] = bnode[d]->d;
        else
            bnode[d+1] = NIL;
        d++;

        while ( d > d_start && ! bnode[d] ) {
            d--;
            ncount--;
            anode = bnode[d]->r;
            bnode[d]->r = nextnode;
            nextnode = bnode[d];
            bnode[d] = anode;
        }
    }

    return( nnode );
}
