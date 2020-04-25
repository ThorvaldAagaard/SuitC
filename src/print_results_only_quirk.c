//  name:
//      print_results_only_quirk
//
//  purpose:
//      print_results_only_quirk prints the results of the plays, but not the tree
//
//  parameters:
//      none
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void print_results_only_quirk( void )
{
    int          d = 0, i, ix, accept, best_MAX, best_MP, best_N[NCARDS];
    double       mpscore, result, perc[NCARDS];
    struct QUIRK *lquirk[NBOARDS];
    struct SCORE *lscore;

    //  initialise
    lquirk[d] = tquirk[0]->d;

    //  loop over all nodes in the tree
    while( lquirk[d] ) {

        //  collect the info on the number of tricks
        nstricks[lquirk[d]->i] = lquirk[d]->ns;

        //  handle the sons
        if ( lquirk[d]->d ) {
            d++;
            lquirk[d] = lquirk[d-1]->d;
        }

        //  process the leaf node
        else {

            //  we reached a leaf; process the results
            if ( lquirk[d]->sc->best_MAX || lquirk[d]->sc->best_MP || lquirk[d]->sc->best_N ) {

                best_MAX = lquirk[d]->sc->result > ( best_result.result - EPS );
                for ( i = 0; i <= maxns - minns; i++ ) {
                    if ( defence )
                        best_N[i] = lquirk[d]->sc->tricks[i] < ( best_result.tricks[i] + EPS ) &&
                                    lquirk[d]->sc->result > ( best_result.res[i] - EPS );
                    else
                        best_N[i] = lquirk[d]->sc->tricks[i] > ( best_result.tricks[i] - EPS ) &&
                                    lquirk[d]->sc->result > ( best_result.res[i] - EPS );
                }
                best_MP =  lquirk[d]->sc->best_MP;

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
                        printf( "\tIMPscore: %8.4f\n", lquirk[d]->sc->mpscore + EPS );

                    //  print the MP score
                    if ( matchpoint && best_MAX && ! best_MP ) {
                        mpscore = 0.0;
                        lscore = score[0].r;
                        while ( lscore->r ) {
                            mpscore -= lscore->perc * lscore->mp[lquirk[d]->sc->ix];
                            lscore = lscore->r;
                        }
                        printf( "\tMPscore: %8.4f\n", 100.0 * mpscore + EPS );
                    }
                }
            }

            //  get the next quirk
            while ( d && ! lquirk[d]->r )
                d--;
            lquirk[d] = lquirk[d]->r;
        }
    }
}
