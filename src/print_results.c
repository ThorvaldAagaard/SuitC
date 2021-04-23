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
//  Copyright 2020, Gerrit A. van der Velde, Leiden

//  #include "header.h"

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
        if ( defence )
            printf( " MIN" );
        else
            printf( " MAX" );
    }
    if ( matchpoint && best_MP )
        printf( " MP" );
    if ( defence ) {
        for ( i = maxns - 1; i >= minns; i-- )
            if ( best_N[i] )
                printf( " L%d", tricklimit - i );
    }
    else {
        for ( i = minns + 1; i <= maxns; i++ )
            if ( best_N[i] )
                printf( " N%d", i);
    }
    printf( "\n=====================" );
    if ( best_MAX )
        printf( "====" );
    if ( matchpoint && best_MP )
        printf( "===" );
    if ( defence ) {
        for ( i = maxns - 1; i >= minns; i-- )
            if ( best_N[i] )
                printf( "===" );
    }
    else {
        for ( i = minns + 1; i <= maxns; i++ )
            if ( best_N[i] )
                printf( "===" );
    }
    printf( "\n\n" );

    if ( defence ) {
        for ( i = maxns; i >= minns; i-- )
            if ( perc[i] < 1.0 -  EPS )
                printf( "\t%d losers: %8.4f\n", tricklimit - i, 100.0 * perc[i] );
        printf( "\tResult  : %8.4f\n", tricklimit - result + EPS );
    }
    else {
        for ( i = maxns; i >= minns; i-- )
            if ( perc[i] > EPS )
                printf( "\t%d tricks: %8.4f\n", i, 100.0 * perc[i] );
        printf( "\tResult  : %8.4f\n", result );
    }

    //  print the MP score
    if ( matchpoint && best_MAX && ! best_MP )
        printf( "\tMP-score: %8.4f\n", 100.0 * mpscore + EPS );
}
