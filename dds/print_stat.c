//  name:
//      print_stat
//
//  purpose:
//      print_stat prints the statistics of an analysis
//
//  parameters:
//      c               in      the print option
//
//  Copyright 2025, Gerrit A. van der Velde, Leiden

#include "header.h"

void print_stat( char c )
{
    int           d;

    times[1] = TICK * clock();
    printf( "\n\nroutine          #calls\n===========    ========\n" );
    printf( "%-11s %11.3f\n", "cpu time", times[1] - times[0] );
    printf( "%-11s %11d\n", "ncount", ncountmax );
    printf( "%-11s %11d\n", "scount", scountmax );
    printf( "%-11s %11d\n", "sqcount", sqcountmax );
    printf( "%-11s %11d\n", "bfill", bfill );
    printf( "%-11s %11d\n", "jfill", jfill );
    printf( "%-11s %11d\n", "sfill", sfill );
    printf( "%-11s %11d\n", "bump_node", ncalls[0] );
    printf( "%-11s %11d\n", "bump_scard", ncalls[1] );
    printf( "%-11s %11d\n", "bump_seqc", ncalls[2] );
    printf( "%-11s %11d\n", "hashbackup", ncalls[8] );
    printf( "%-11s %11d\n", "join_scard", ncalls[9] );
    if ( c == 'P' ) {
        printf( "\n d     count\n==    ======\n" );
            for ( d = 1; d <= 4*ntricks; d++ )
                printf( "%2d%10d\n", d, st[d].n );
    }

    exit( 0 );
}
