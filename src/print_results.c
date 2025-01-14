//  name:
//      print_results
//
//  purpose:
//      print_results prints the results of the currently selected play
//
//  parameters:
//      best_MAX        in      indicator for MAX
//      best_MP         in      indicator for MATCHPOINT
//      best_N          in      indicator for N tricks
//      mpscore         in      the MP or IMP score of this play
//
//  Copyright 2024, Gerrit A. van der Velde, Leiden

#include "header.h"

void print_results( int best_MAX, int best_MP, int *best_N, double mpscore )
{
    int          i, n;
    double       result, perc[NCARDS+1];
    struct SCORE *lscore;

    result = 0.0;
    for ( i = minns; i <= maxns; i++ )
        perc[i] = 0.0;

    for ( n = 0; n < boardcount; n++ ) {
        perc[nstricks[n]] += dupl[n] * prob[n];
        result += dupl[n] * prob[n] * nstricks[n];
    }

    printf( "\n\n\nThe optimum play for:" );
    if ( best_MAX ) {
        if ( ewwinners )
            printf( " MIN" );
        else
            printf( " MAX" );
    }
    if ( matchpoint && best_MP )
        printf( " %s", matchpoint == IMP ? "IMP" : "MP" );
    if ( ewwinners ) {
        for ( i = maxns; i >= minns; i-- )
            if ( best_N[i] )
                printf( " L%d", tricklimit - i );
    }
    else if ( defence ) {
        for ( i = MAX(minns,1); i <= maxns; i++ )
            if ( best_N[i] )
                printf( " S%d", i);
    }
    else {
        for ( i = MAX(minns,1); i <= maxns; i++ )
            if ( best_N[i] )
                printf( " N%d", i);
    }
    printf( "\n=====================" );
    if ( best_MAX )
        printf( "====" );
    if ( matchpoint && best_MP )
        printf( "%s", matchpoint == IMP ? "====" : "===" );
    if ( ewwinners ) {
        for ( i = maxns; i >= minns; i-- ) {
            if ( best_N[i] ) {
                printf( "===" );
                if (  tricklimit - i >= 10 )
                    printf( "=" );
            }
        }
    }
    else {
        for ( i = MAX(minns,1); i <= maxns; i++ ) {
            if ( best_N[i] ) {
                printf( "===" );
                if ( i >= 10 )
                    printf( "=" );
            }
        }
    }
    printf( "\n\n" );

    if ( ewwinners ) {
        for ( i = maxns; i >= minns; i-- )
            printf( "\t%d losers: %8.4f\n", tricklimit - i, 100.0 * perc[i] );
        printf( "\tResult  : %8.4f\n", tricklimit - result + EPS );
    }
    else {
        for ( i = maxns; i >= minns; i-- )
            printf( "\t%d tricks: %8.4f\n", i, 100.0 * perc[i] );
        printf( "\tResult  : %8.4f\n", result );
    }

    //  print the IMP or MP score
    if ( matchpoint && ( best_MAX && best_MP == NO || printallmatchpoint ) )
        printf( "\t%sscore: %8.4f\n", matchpoint == IMP ? "IMP" : "MP-", mpscore + EPS );
}
