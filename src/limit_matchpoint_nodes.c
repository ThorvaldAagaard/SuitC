//  name:
//      limit_matchpoint_nodes
//
//  purpose:
//      limit_matchpoint_nodes limits the number of NODES for the MP evaluation.
//
//  parameters:
//      none
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void limit_matchpoint_nodes( void )
{
    int          count, ix;
    double       testlow, testval, testhigh;
    struct SCORE *lscore;

    //  find the lowest result
    testlow = 100.0 * ntricks;
    lscore = score[0].r;
    while ( lscore->r ) {
        testlow = MIN( testlow, lscore->result );
        lscore = lscore->r;
    }

    testhigh = best_result.result;
    while ( ( testhigh - testlow ) > 0.01 ) {

        //  count the number of accepted plays
        count = 0;
        testval = 0.5 * ( testhigh + testlow );
        lscore = score[0].r;
        while ( lscore->r ) {
            if ( lscore->result > ( testval - EPS ) )
                count++;
            lscore = lscore->r;
        }

        if ( count < maxnodes )
            testhigh = testval;
        else
            testlow = testval;
    }

    //  count the number of potential MP nodes and renumber the scores
    ix = 0;
    lscore = score[0].r;
    while ( lscore->r ) {
        if ( lscore->best_MAX || lscore->best_N || ( lscore->result > ( testlow - EPS ) ) )
            lscore->ix = ix++;
        else if ( lscore->result < ( testlow - EPS ) ) {
            lscore->best_MP = NO;
            if ( lscore->best_MAX || lscore->best_N )
                lscore->ix = ix++;
            else {
                lscore->r->l = lscore->l;
                lscore->l->r = lscore->r;
            }
        }
        lscore = lscore->r;
    }

    //  delete all invalidated quirks and recalcualte the results
    if ( qhd == 1 ) {
        clean_quirk( tquirk[0] );
        nplays = count_quirk( tquirk[0]->d );
        evaluate_all_results_quirk();
    }
    if ( qhd == 2 ) {
        clean_quirkh( &tquirkd[0] );
        count_quirkh( &tquirkd[0] );
        evaluate_all_results_quirkh();
    }

    printf( "\n\t#nodes for MP plays : %d\n", nplays );
    printf( "\tcutoff for MP plays : %6.4f\n\n", best_result.result - testlow ); fflush( stdout );
}
