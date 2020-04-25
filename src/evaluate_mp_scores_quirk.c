//  name:
//      evaluate_mp_scores_quirk
//
//  purpose:
//      evaluate_mp_scores_quirk determines the optimal plays in case that
//      the matchpoint score is taken into account.
//
//  parameters:
//      none
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void evaluate_mp_scores_quirk( void )
{
    int           b, d = 0, i;
    int           imptable[] = { 10, 40, 80, 120, 160, 210, 260, 310, 360, 420, 490, 590, 740, 890,
                                 1090, 1290, 1490, 1740, 1990, 2240, 2490, 2990, 3490, 3990 };
    double        mpscore[NBOARDS];
    struct QUIRK  *bquirk[NBOARDS+1], *lquirk[NBOARDS+1];
    struct SCORE  *ascore, *lscore;

    //  initialise
    lquirk[d] = tquirk[0] ? tquirk[0]->d : NIL;

    //  loop over the quirk tree
    if ( trace & 0x2 )
        printf( "\n" );
    while ( lquirk[d] ) {

        //  get the next node
        if ( lquirk[d]->d ) {
            d++;
            lquirk[d] = lquirk[d-1]->d;
        }

        //  we reached a leaf
        else {

            //  allocate memory for the matchpoint scores
            lscore = lquirk[d]->sc;
            lscore->mp = malloc( nplays * sizeof( double ) );
            memory += nplays * sizeof( double );
            mp_memory = memory > mp_memory ? memory : mp_memory;
            if ( ! lscore->mp ) {
                printf( "\nERROR: No more memory available: %6.1f MB after %3.1f seconds\n" , (float) memory / 1048576, TICK * clock() );
                exit( 1 );
            }

            //  perform the second loop over the quirk tree
            b = 0;
            bquirk[b] = tquirk[0] ? tquirk[0]->d : NIL;
            while ( bquirk[b] ) {

                //  calculate the matchpoint score
                if ( matchpoint )
                    mpscore[b] = ( lquirk[b]->ns > bquirk[b]->ns ? 1.0 : ( lquirk[b]->ns < bquirk[b]->ns ? -1.0 : 0.0 ) );
                else if ( impscore ) {
                    mpscore[b] = pointscore[lquirk[b]->ns] - pointscore[bquirk[b]->ns];
                    mpscore[b] = mpscore[b] >= 0 ? mpscore[b] : -mpscore[b];
                    i = 24;
                    while ( i && imptable[i-1] >= mpscore[b] )
                        i--;
                    mpscore[b] = ( pointscore[lquirk[b]->ns] >= pointscore[bquirk[b]->ns] ? i : -i );
                }
                mpscore[b] = prob[b] * dupl[b] * mpscore[b];
                mpscore[b] += ( b ? mpscore[b-1] : 0.0 );

                //  get the next node
                if ( bquirk[b]->d ) {
                    b++;
                    bquirk[b] = bquirk[b-1]->d;
                }

                //  we reached a leaf; store the matchpoint score
                else {
                    lquirk[d]->sc->mp[bquirk[b]->sc->ix] = mpscore[b];
                    bquirk[b] = bquirk[b]->r;
                }

                //  get the next node
                while ( b && ! bquirk[b] ) {
                    b--;
                    bquirk[b] = bquirk[b]->r;
                }
            }

            //  compare the scores and unlink dominated ones
            check_zermelo( lscore );

            lquirk[d] = lquirk[d]->r;
        }

        //  get the next node
        while ( d && ! lquirk[d] ) {
            d--;
            lquirk[d] = lquirk[d]->r;
        }
    }
}
