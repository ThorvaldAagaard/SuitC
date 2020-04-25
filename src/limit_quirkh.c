//  name:
//      limit_quirkh
//
//  purpose:
//      limit_quirkh limits the number of branches in the QUIRKH trees
//      that are kept during the merging process. QUIRKH branches are deleted
//      untill the product of the number of QUIRKH branches in tquirkd[d] and
//      tquirkd[d+1] is about the value of cutoff.
//
//  parameters:
//      d               in      the depth in the NODE tree
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void limit_quirkh( int d )
{
    int           i, k, count[2], quirkhcount[2];
    double        result[2], testlow, testval, testhigh;
    struct QUIRKD *lquirkd[2];
    struct SCORE  *lscore, *score[2];

    //  evaluate results for both nodes.
    testlow = 100.0 * ntricks;
    testhigh = 0.0;
    lquirkd[0] = &tquirkd[d];;
    lquirkd[1] = &tquirkd[d+1];
    for ( i = 0; i < 2; i++ ) {

        //  count the number of terminal nodes
        quirkhcount[i] = count_quirkh( lquirkd[i] );

        //  reserve memory for the score entries
        score[i] = (struct SCORE *) malloc( ( quirkhcount[i] + 2 ) * sizeof( struct SCORE ) );
        memory += ( quirkhcount[i] + 2 ) * sizeof( struct SCORE );
        if ( ! score[i] ) {
            printf( "\nERROR: No more memory available: %6.1f MB after %3.1f seconds\n" , (float) memory / 1048576, TICK * clock() );
            exit( 1 );
        }

        //  create the links for the score entries
        score[i][0].l = NIL;
        for ( k = 0; k <= quirkhcount[i]; k++ ) {
            score[i][k+1].l = &score[i][k];
            score[i][k].r = &score[i][k+1];
            score[i][k].b = NIL;
        }
        score[i][quirkhcount[i]+1].r = NIL;

        //  evaluate the results of all plays
        evaluate_results_only_quirkh( lquirkd[i], &score[i][1] );
        result[i] = best_result.result;

        //  determine the limits of the search process
        lscore = score[i][0].r;
        while ( lscore->r ) {
            testlow = MIN( testlow, result[i] - lscore->result );
            testhigh = MAX( testhigh, result[i] - lscore->result );
            lscore = lscore->r;
        }
    }

    //  determine the cutoff level
    testval = 0.5 * ( testhigh + testlow );
    for ( i = 0; i < 2; i++ )
        count[i] = quirkhcount[i];
    while ( ( testhigh - testlow ) > 0.0001 ) {

        //  count the number of plays
        for ( i = 0; i < 2; i++ ) {
            count[i] = 0;
            lscore = score[i][0].r;
            while ( lscore->r ) {
                if ( ( result[i] - lscore->result - EPS ) < testval )
                    count[i]++;
                lscore = lscore->r;
            }
        }

        //  determine the new limits for the search
        if ( ( (long long) count[0] * count[1] ) < cutoff )
            testlow = testval;
        else
            testhigh = testval;
        testval = 0.5 * ( testhigh + testlow );
    }

    //  remove unaccepted plays from the list and clean up
    for ( i = 0; i < 2; i++ ) {

        lscore = score[i][0].r;
        while ( lscore->r ) {
            if ( ( result[i] - lscore->result - EPS ) > testval ) {
                lscore->best_MAX = NO;
                lscore->best_MP = NO;
                lscore->best_N = NO;
            }
            lscore = lscore->r;
        }

        //  delete all plays that cannot result in the best scores
        clean_quirkh( lquirkd[i] );

        free( score[i] );
        memory -= ( quirkhcount[i] + 2 ) * sizeof( struct SCORE );
    }

    printf( "\tcutoff:%8d%6d ->%8d%6d\n", quirkhcount[0], quirkhcount[1], count[0], count[1] );
    fflush( stdout );
}
