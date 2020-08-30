//  name
//      select_ew_plays
//
//  purpose:
//      select_ew_plays selects the north-south plays that wins tricks as early as possible
//
//  parameters:
//      d               in      the depth in the NODE tree
//
//  Copyright 2020, Gerrit A. van der Velde, Leiden

#include "header.h"

void select_ew_plays( int d )
{
    struct NODE  *bnode[NPLIES+1], *lnode[NPLIES+1];

    //  initialise
    bnode[d-1] = tnode[d-1];
    lnode[d-1] = tnode[d-1];
    lnode[d] = tnode[d-1]->d;

    //  investigate the sons
    if ( tnode[d-1]->d->d ) {
        tnode[d] = tnode[d-1]->d;
        while ( tnode[d] ) {
            select_ew_plays( d+1 );
            tnode[d] = tnode[d]->r;
        }
    }

    if ( NS(lnode[d]->p) ) {

        //  compare plays by the same player
    next_lnode:
        while ( lnode[d] && lnode[d]->r ) {

            bnode[d] = lnode[d]->r;
            while ( bnode[d] ) {

                if ( lnode[d]->p == bnode[d]->p ) {

                    //  select the card that minimises the number of east-west cards that can win the trick
                    if ( w_ew(lnode) > w_ew(bnode) ) {
                        lnode[d] = delete_tree( d, lnode );
                        goto next_lnode;
                    }
                    else if ( w_ew(lnode) < w_ew(bnode) )
                        bnode[d] = delete_tree( d, bnode );
                    else
                        bnode[d] = bnode[d]->r;
                }

                else
                    bnode[d] = bnode[d]->r;
            }

            lnode[d] = lnode[d]->r;
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
