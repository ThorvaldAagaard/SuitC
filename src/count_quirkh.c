//  name:
//      count_quirkh
//
//  purpose:
//      count_quirkh counts the number possible plays
//
//  parameters:
//      rootqd           in      the root of the QUIRKH tree
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

int count_quirkh( struct QUIRKD *rootqd )
{
    int           k, l = 0;
    struct QUIRKD *lquirkd[NBOARDS], *lquirkdl[NBOARDS];

    //  initialise
    maxns = 0;
    minns = tricklimit;
    nplays = 0;
    lquirkdl[l] = rootqd;
    lquirkd[l] = lquirkdl[l]->h->d;

    while ( lquirkd[l] ) {

        for ( k = lquirkd[l]->il; k <= lquirkd[l]->ih; k++ ) {
            minns = MIN( minns, lquirkdl[l]->h->t[k].ns );
            maxns = MAX( maxns, lquirkdl[l]->h->t[k].ns );
        }

        if ( lquirkd[l]->d ) {
            l++;
            lquirkdl[l] = lquirkdl[l-1];
            lquirkd[l] = lquirkd[l-1]->d;
        }

        else {
            nplays++;
            while ( l && lquirkd[l]->h == NIL )
                l--;
            lquirkdl[l] = lquirkd[l];
            lquirkd[l] = lquirkd[l]->h ? lquirkd[l]->h->d : NIL;
        }
    }

    return( nplays );
}
