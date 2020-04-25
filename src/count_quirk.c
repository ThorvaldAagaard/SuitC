//  name:
//      count_quirk
//
//  purpose:
//      count_quirk counts the number possible plays
//
//  parameters:
//      rootq            in      the source quirk
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

int count_quirk( struct QUIRK *rootq )
{
    int          d = 0, lcount = 0;
    struct QUIRK *lquirk[NBOARDS];

    //  initialise
    maxns = 0;
    minns = tricklimit;
    lquirk[d] = rootq;

    //  loop over all nodes in the tree
    while( lquirk[d] ) {

        //  determine the minimum and maximum number of tricks won by north-south
        if ( lquirk[d]->i != -1 ) {
            minns = MIN( minns, lquirk[d]->ns );
            maxns = MAX( maxns, lquirk[d]->ns );
        }

        //  count the leaves and get the next node
        if ( lquirk[d]->d ) {
            d++;
            lquirk[d] = lquirk[d-1]->d;
        }
        else {
            lcount++;
            while ( d && ! lquirk[d]->r )
                d--;
            lquirk[d] = lquirk[d]->r;
        }
    }

    return( lcount );
}
