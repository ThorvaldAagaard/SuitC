//  name
//      select_ns_plays
//
//  purpose:
//      select_ns_plays selects the favoured human oriented plays
//
//  parameters:
//      d               in      the depth in the NODE tree
//
//  Copyright 2020, Gerrit A. van der Velde, Leiden

#include "header.h"

void select_ns_plays( int d )
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
            select_ns_plays( d+1 );
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

                    //  in 3rd or 4th hand follow suit with the lowest card
                    else if ( (d-1)%4+1 >= 3 ) {
                        lnode[d] = delete_tree( d, lnode );
                        goto next_lnode;
                    }

                    //  select the lowest lead from equal cards or select the lead that wins more often
                    else if ( w_ns(lnode) >= w_ns(bnode) ) {
                        lnode[d] = delete_tree( d, lnode );
                        goto next_lnode;
                    }
                    else if ( w_ns(lnode)  < w_ns(bnode) )
                        bnode[d] = delete_tree( d, bnode );

                    else if ( YES ) {
                        printf( "\nply=%02d lnode[d]: c=%04x winc=%04x c&winc=%04x w_ew=%04x w_ns=%04x\n       bnode[d]: c=%04x winc=%04x c&winc=%04x w_ew=%04x w_ns=%04x\n",
                                d, lnode[d]->c >> 2, lnode[d]->winc >> 2, ( lnode[d]->c & lnode[d]->winc ) >> 2, w_ew(lnode) >> 2, w_ns(lnode) >> 2,
                                   bnode[d]->c >> 2, bnode[d]->winc >> 2, ( bnode[d]->c & bnode[d]->winc ) >> 2, w_ew(bnode) >> 2, w_ns(bnode) >> 2 );
                        (d-1)%4+1 >= 3 ? print_tree( d-2, tnode[d-3]->d, NO, YES ) : print_tree( d, tnode[d-1]->d, NO, YES );
                        bnode[d] = bnode[d]->r;
                    }
                }

                else
                    bnode[d] = bnode[d]->r;
            }

            lnode[d] = lnode[d]->r;
        }

        //  compare plays by different players
        lnode[d] = tnode[d-1]->d;
        bnode[d] = lnode[d]->r;
        if ( lnode[d] && bnode[d] ) {

            //  select the card that minimises the number of east-west cards that can win the trick
            if ( w_ew(lnode) > w_ew(bnode) )
                lnode[d] = delete_tree( d, lnode );
            else if ( w_ew(lnode) < w_ew(bnode) )
                bnode[d] = delete_tree( d, bnode );

            //  lead the highest card
            else if ( lnode[d]->c > bnode[d]->c )
                bnode[d] = delete_tree( d, bnode );
            else if ( lnode[d]->c < bnode[d]->c )
                lnode[d] = delete_tree( d, lnode );

            else if ( YES ) {
                printf( "\nply=%02d lnode[d]: c=%04x winc=%04x c&winc=%04x w_ew=%04x w_ns=%04x\n       bnode[d]: c=%04x winc=%04x c&winc=%04x w_ew=%04x w_ns=%04x\n",
                        d, lnode[d]->c >> 2, lnode[d]->winc >> 2, ( lnode[d]->c & lnode[d]->winc ) >> 2, w_ew(lnode) >> 2, w_ns(lnode) >> 2,
                           bnode[d]->c >> 2, bnode[d]->winc >> 2, ( bnode[d]->c & bnode[d]->winc ) >> 2, w_ew(bnode) >> 2, w_ns(bnode) >> 2 );
                (d-1)%4+1 >= 3 ? print_tree( d-2, tnode[d-3]->d, NO, YES ) : print_tree( d, tnode[d-1]->d, NO, YES );
                bnode[d] = bnode[d]->r;
            }
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
