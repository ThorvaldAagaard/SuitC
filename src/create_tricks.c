//  name:
//      create_tricks
//
//  purpose:
//      create_tricks is the driver program for creating tricks. First it
//      checks the if still more tricks have to be played or if restrictions
//      apply on who can lead. If more tricks have to be played it creates
//      the setup for the next trick.
//
//  parameters:
//      d               in      the depth in the tree
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void create_tricks( int d )
{
    int          i, lead[4], p, q, t = st[d].t, throwinback = throwin;
    COLOR        c;
    struct INDEX *lindex;
    struct SCARD *sc;

    //  collect the remaining cards
    for ( p = 0; p < 4; p++ )
        brd[t][p] = RENONCE;
    lindex = wnode[d-1]->i;
    while( lindex ) {
        for ( p = 0; p < 4; p++ )
            brd[t][p] |= lindex->c[p];
        lindex = lindex->r;
    }

    //  determine who can lead to this trick
    for ( p = 0; p < 4; p++ )
        lead[p] = NO;
    if ( defence ) {
        lead[west] = brd[t][west] && ( w[t-1] == west || w[t-1] == noplayer || ( t > 1 && nfree[t-1][west] ) );
        lead[east] = brd[t][east] && ( w[t-1] == east || w[t-1] == noplayer || ( t > 1 && nfree[t-1][east] ) );
    }
    if ( throwin && t > 1 && ( NS( w[t-1] ) || ( w[t-1] == west && ! brd[t][west] ) || ( w[t-1] == east && ! brd[t][east] ) ) )
        throwin = NO;
    if ( throwin ) {
        lead[west] = brd[t][west] && ( w[t-1] == west || w[t-1] == noplayer );
        lead[east] = brd[t][east] && ( w[t-1] == east || w[t-1] == noplayer );
    }
    if ( ! ( defence || throwin ) ) {
        lead[north] = brd[t][north] && ( w[t-1] == north || w[t-1] == noplayer || ( t > 1 && nfree[t-1][north] ) );
        lead[south] = brd[t][south] && ( w[t-1] == south || w[t-1] == noplayer || ( t > 1 && nfree[t-1][south] ) );
    }

    //  no more tricks are to be played
    if ( t == ntricks || ! ( lead[west] | lead[north] | lead[east] || lead[south] ) ) {

        wnode[d-1]->term = YES;

        if ( ( lead[west] | lead[north] | lead[east] || lead[south] ) && t == ntricks ) {

            ns[t] = ns[t-1];
            p = brd[t][north] > brd[t][east] ? north : east;
            p = brd[t][south] > brd[t][p]    ? south : p;
            p = brd[t][west]  > brd[t][p]    ? west  : p;
            for ( q = 0; q < 4; q++ )
                if ( nruff[t-1][q] && ! brd[t][q] )
                    p = q;
            ns[t] += ( defence ? ( NS( p ) ? 0 : -1 ) : ( NS( p ) ? 1 : 0 ) );

            lindex = wnode[d-1]->i;
            while( lindex ) {
                lindex->min = ns[t];
                lindex->max = lindex->min;
                lindex = lindex->r;
            }
        }

        else if ( throwin ) {

            ns[t] = ns[t-1] + tricklimit - t + 1;
            lindex = wnode[d-1]->i;
            while( lindex ) {
                lindex->min = ns[t];
                lindex->max = lindex->min;
                lindex = lindex->r;
            }
        }

        return;
    }

    //  complete the hands by adding equivalent cards
    for ( p = 0; p < 4; p++ ) {
        sc = &scards[0][p];
        for ( i = 0; i <= sc->n; i++ )
            if ( brd[t][p] & sc->c[i] )
                brd[t][p] |= sc->c[i];
    }

    //  initialise
    for ( p = 0; p < 4; p++ ) {
        sc = &scards[t][p]; sc->n = 0;
        if ( nruff[t][p] && ( ncardsmin[p] < t ) )
            sc->c[sc->n++] = addcard(ruff);
        sc->c[sc->n] = (COLOR) 0;
    }

    //  lead from each hand in turn
    create_ply1( d, lead );

    throwin = throwinback;
}
