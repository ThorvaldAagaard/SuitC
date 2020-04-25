//  name:
//      print_results
//
//  purpose:
//      print_results prints the results of the siblings at play=1
//
//  parameters:
//      best_MAX        in      indicator for MAX
//      best_MP         in      indicator for MATCHPOINT
//      best_N          in      indicator for N tricks
//      mpscore         in      the matchpoint score of this play
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void print_results( int best_MAX, int best_MP, int *best_N, double mpscore )
{
    int          i, n;
    double       result, perc[NCARDS];
    struct SCORE *lscore;

    result = 0.0;
    for ( i = 0; i <= maxns - minns; i++ )
        perc[i] = 0.0;

    for ( n = 0; n < boardcount; n++ ) {
        perc[nstricks[n]-minns] += dupl[n] * prob[n];
        result += dupl[n] * prob[n] * nstricks[n];
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
                printf( " L%d", tricklimit - i - minns);
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
        for ( i = maxns - minns; i >= 0; i-- )
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

    //  print the IMP or MP score
    if ( impscore && best_MAX && ! best_MP )
        printf( "\tIMPscore: %8.4f\n", mpscore + EPS );
    if ( matchpoint && best_MAX && ! best_MP )
        printf( "\tMP-score: %8.4f\n", 100.0 * mpscore + EPS );

    printf( "\n" );
    if ( tnode[0]->d )
        print_tree( 1, tnode[0]->d, YES, YES );
    else
        print_tree( 0, tnode[0], YES, YES );
}
