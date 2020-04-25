//  name:
//      print_results_only_quirkh
//
//  purpose:
//      print_results_only_quirkh prints the results of the plays, but not the tree.
//      this version uses QUIRKH trees.
//
//  parameters:
//      none
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void print_results_only_quirkh( void )
{
    int           d = 0, i, ix, k, accept, best_MAX, best_MP, best_N[NCARDS];
    double        mpscore, result, perc[NCARDS];
    struct QUIRKD *lquirkd[NBOARDS+1];
    struct QUIRKH *lquirkh[NBOARDS+1];
    struct SCORE  *lscore;

    //  initialise
    lquirkh[d] = tquirkd[d].h;
    lquirkd[d] = lquirkh[d]->d;

    //  loop over all nodes in the tree
    while( lquirkd[d] ) {

        //  collect the info on the number of tricks
        for ( k = lquirkd[d]->il; k <= lquirkd[d]->ih; k++ )
             nstricks[lquirkh[d]->t[k].i] = lquirkh[d]->t[k].ns;

        //  handle the sons
        if ( lquirkd[d]->d ) {
            d++;
            lquirkh[d] = lquirkh[d-1];
            lquirkd[d] = lquirkd[d-1]->d;
        }

        //  process the leaf node
        else {

            //  we reached a leaf; process the results
            if ( lquirkh[d]->sc->best_MAX || lquirkh[d]->sc->best_MP || lquirkh[d]->sc->best_N ) {

                best_MAX = lquirkh[d]->sc->result > ( best_result.result - EPS );
                for ( i = 0; i <= maxns - minns; i++ ) {
                    if ( defence )
                        best_N[i] = lquirkh[d]->sc->tricks[i] < ( best_result.tricks[i] + EPS ) &&
                                    lquirkh[d]->sc->result > ( best_result.res[i] - EPS );
                    else
                        best_N[i] = lquirkh[d]->sc->tricks[i] > ( best_result.tricks[i] - EPS ) &&
                                    lquirkh[d]->sc->result > ( best_result.res[i] - EPS );
                }
                best_MP =  lquirkh[d]->sc->best_MP;

                //  check if any optimal play is left
                accept = NO;
                if ( best_MAX || best_MP )
                    accept = YES;
                for ( i = 0; i <= maxns - minns; i++ )
                    if ( best_N[i] )
                        accept = YES;

                if ( accept ) {

                    result = 0.0;
                    for ( i = 0; i <= maxns - minns; i++ )
                        perc[i] = 0.0;
                    for ( i = 0; i < boardcount; i++ ) {
                        perc[nstricks[i]-minns] += dupl[i] * prob[i];
                        result += dupl[i] * prob[i] * nstricks[i];
                    }

                    printf( "\n\n\nThe optimum play for:" );
                    if ( best_MAX ) {
                        if ( defence )
                            printf( " MIN" );
                        else
                            printf( " MAX" );
                    }
                    if ( impscore && best_MP )
                        printf( " IMP" );
                    if ( matchpoint && best_MP )
                        printf( " MP" );
                    if ( defence ) {
                        for ( i = maxns - minns - 1; i >= 0; i-- )
                            if ( best_N[i] )
                                printf( " L%d", tricklimit - i - minns );
                    }
                    else {
                        for ( i = 1; i <= maxns - minns; i++ )
                            if ( best_N[i] )
                                printf( " N%d", i + minns );
                    }
                    printf( "\n=====================" );
                    if ( best_MAX )
                        printf( "====" );
                    if ( impscore && best_MP )
                        printf( "====" );
                    if ( matchpoint && best_MP )
                        printf( "===" );
                    if ( defence ) {
                        for ( i = maxns - minns - 1; i >= 0; i-- )
                            if ( best_N[i] )
                                printf( "===" );
                    }
                    else {
                        for ( i = 1; i <= maxns - minns; i++ )
                            if ( best_N[i] )
                                printf( "===" );
                    }
                    printf( "\n\n" );

                    if ( defence ) {
                        for ( i = maxns - minns - 1; i >= 0; i-- )
                            if ( perc[i] < 1.0 -  EPS )
                                printf( "\t%d losers: %8.4f\n", tricklimit - i - minns, 100.0 * perc[i] );
                        printf( "\tResult  : %8.4f\n", tricklimit - result + EPS );
                    }
                    else {
                        for ( i = maxns - minns; i >= 0; i-- )
                            if ( perc[i] > EPS )
                                printf( "\t%d tricks: %8.4f\n", i + minns, 100.0 * perc[i] );
                        printf( "\tResult  : %8.4f\n", result );
                    }

                    //  print the IMP score
                    if ( impscore && best_MAX && ! best_MP )
                        printf( "\tIMPscore: %8.4f\n", lquirkh[d]->sc->mpscore + EPS );

                    //  print the MP score
                    if ( matchpoint && best_MAX && ! best_MP ) {
                        mpscore = 0.0;
                        lscore = score[0].r;
                        while ( lscore->r ) {
                            mpscore -= lscore->perc * lscore->mp[lquirkh[d]->sc->ix];
                            lscore = lscore->r;
                        }
                        printf( "\tMPscore: %8.4f\n", 100.0 * mpscore + EPS );
                    }
                }
            }

            //  get the next quirk
            while ( d && ! lquirkd[d]->h )
                d--;
            lquirkh[d] = lquirkd[d]->h;
            lquirkd[d] = lquirkh[d] ? lquirkh[d]->d : NIL;
        }
    }
}
