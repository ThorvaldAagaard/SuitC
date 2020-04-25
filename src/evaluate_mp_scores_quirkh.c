//  name:
//      evaluate_mp_scores_quirkh
//
//  purpose:
//      evaluate_mp_scores_quirkh determines the optimal plays in case that
//      the matchpoint score is taken into account.
//
//  parameters:
//      none
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void evaluate_mp_scores_quirkh( void )
{
    int           b, d = 0, i, ix, k, v;
    int           imptable[] = { 10, 40, 80, 120, 160, 210, 260, 310, 360, 420, 490, 590, 740, 890,
                                 1090, 1290, 1490, 1740, 1990, 2240, 2490, 2990, 3490, 3990 };
    double        mpscore[NBOARDS];
    struct QUIRKD *bquirkd[NBOARDS+1], *lquirkd[NBOARDS+1];
    struct QUIRKH *bquirkh[NBOARDS+1], *lquirkh[NBOARDS+1];
    struct SCORE  *ascore, *lscore;

    //  initialise
    lquirkh[d] = tquirkd[d].h;
    lquirkd[d] = lquirkh[d] ? lquirkh[d]->d : NIL;

    //  loop over the quirk tree
    if ( trace & 0x2 )
        printf( "\n" );
    while ( lquirkd[d] ) {

        //  get the next node
        d++;
        lquirkh[d] = lquirkh[d-1];
        lquirkd[d] = lquirkd[d-1]->d;
        if ( lquirkd[d] ) continue;

        //  we reached a leaf
        //  allocate memory for the matchpoint scores
        d--;
        lscore = lquirkh[d]->sc;
        lscore->mp = malloc( nplays * sizeof( double ) );
        memory += nplays * sizeof( double );
        mp_memory = memory > mp_memory ? memory : mp_memory;
        if ( ! lscore->mp ) {
            printf( "\nERROR: No more memory available: %6.1f MB after %3.1f seconds\n" , (float) memory / 1048576, TICK * clock() );
            exit( 1 );
        }
        for ( ix = 0; ix < nplays; ix++ )
            lscore->mp[ix] = 0.0;

        //  perform the second loop over the quirk tree
        b = 0;
        bquirkh[b] = tquirkd[b].h;
        bquirkd[b] = bquirkh[b] ? bquirkh[b]->d : NIL;
        while ( bquirkd[b] ) {

            //  calculate the matchpoint score
            v = 0;
            while ( lquirkd[v]->ih < bquirkd[b]->il )
                v++;
            for ( k = bquirkd[b]->il; k <= bquirkd[b]->ih; k++ ) {
                if ( lquirkd[v]->ih < k ) v++;
                if ( matchpoint )
                    mpscore[k] = ( lquirkh[v]->t[k].ns > bquirkh[b]->t[k].ns ? 1.0 : ( lquirkh[v]->t[k].ns < bquirkh[b]->t[k].ns ? -1.0 : 0.0 ) );
                else if ( impscore ) {
                    mpscore[k] = pointscore[lquirkh[v]->t[k].ns] - pointscore[bquirkh[b]->t[k].ns];
                    mpscore[k] = mpscore[k] >= 0 ? mpscore[k] : -mpscore[k];
                    i = 24;
                    while ( i && imptable[i-1] >= mpscore[k] )
                        i--;
                    mpscore[k] = ( pointscore[lquirkh[v]->t[k].ns] >= pointscore[bquirkh[b]->t[k].ns] ? i : -i );
                }
                mpscore[k] = prob[k] * dupl[k] * mpscore[k];
                mpscore[k] += ( k ? mpscore[k-1] : 0.0 );
            }

            //  get the next node
            b++;
            bquirkh[b] = bquirkh[b-1];
            bquirkd[b] = bquirkd[b-1]->d;
            if ( bquirkd[b] ) continue;

            //  we reached a leaf; store the matchpoint score
            b--;
            lscore->mp[bquirkh[b]->sc->ix] = mpscore[bquirkd[b]->ih];

            //  get the next node
            b++;
            while ( b && ! bquirkd[b] ) {
                b--;
                bquirkh[b] = bquirkd[b]->h;
                bquirkd[b] = bquirkh[b] ? bquirkh[b]->d : NIL;
            }
        }

        //  compare the scores and unlink dominated ones
        check_zermelo( lscore );

        //  get the next node
        d++;
        while ( d && ! lquirkd[d] ) {
            d--;
            lquirkh[d] = lquirkd[d]->h;
            lquirkd[d] = lquirkh[d] ? lquirkh[d]->d : NIL;
        }
    }
}
