//  name:
//      fold_ns_tree
//
//  purpose:
//      fold_ns_tree folds the tree when east-west branches are merged
//
//  parameters:
//      d               in      the depth in the NODE tree
//      c               in      the cards that will be merged
//      lnode           in      the first sibling NODE tree
//      bnode           in      the second sibling NODE tree
//
//  Copyright 2020, Gerrit A. van der Velde, Leiden

#include "header.h"

int fold_ns_tree( int d, COLOR c, struct NODE **lnode, struct NODE **bnode )
{
    int          match;

    lnode[d-1]->match = NO;
    bnode[d] = bnode[d-1]->d;
    lnode[d] = lnode[d-1]->d;
    if ( EW(lnode[d]->p) ) {

        while ( lnode[d] ) {

            match = NO;
            bnode[d] = bnode[d-1]->d;
            while ( bnode[d] ) {
                if ( lnode[d]->p == bnode[d]->p && lnode[d]->c == bnode[d]->c ) {
                    if ( ! bnode[d]->d || fold_ns_tree( d+1, c, lnode, bnode ) ) {
                        match = YES;
                        break;
                    }
                }
                bnode[d] = bnode[d]->r;
            }

            if ( ! match )
                return( NO );
            lnode[d] = lnode[d]->r;
        }
    }

    else if ( NS(lnode[d]->p) ) {

        while ( lnode[d] ) {

            match = NO;
            bnode[d] = bnode[d-1]->d;
            while ( bnode[d] ) {

                if ( lnode[d]->p == bnode[d]->p && ( lnode[d]->c == bnode[d]->c || ( c & lnode[d]->c && c & bnode[d]->c ) ) ) {
                    if ( ! bnode[d]->d || fold_ns_tree( d+1, c, lnode, bnode ) ) {
                        match = YES;
                        lnode[d]->c = c & lnode[d]->c && c & bnode[d]->c ? c : lnode[d]->c;
                    }
                }

                bnode[d] = bnode[d]->r;
            }

            if ( ! match )
                return( NO );
            lnode[d] = lnode[d]->r;
        }
    }

    //  determine the winning cards
    lnode[d] = lnode[d-1]->d;
    lnode[d-1]->winc = (d-1)%4 ? RENONCE : lnode[d-1]->winc;
    while ( lnode[d] ) {
        if ( (d-1)%4 )
            lnode[d-1]->winc |= lnode[d]->winc;
        lnode[d] = lnode[d]->r;
    }

    return( YES );
}
