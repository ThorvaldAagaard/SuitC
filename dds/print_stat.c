//  name:
//      print_stat
//
//  purpose:
//      print_stat prints the statistics of an analysis
//
//  parameters:
//      c               in      the print option
//
//  Copyright 2024, Gerrit A. van der Velde, Leiden

#include "header.h"

void print_stat( char c )
{
    int           d;

    times[1] = TICK * clock();
    printf( "\n\ncpu time =%10.3f\n", times[1] - times[0] );
    printf( "ncount   =%10d\n", ncountmax );
    printf( "scount   =%10d\n", scountmax );
    printf( "bfill    =%10d\n", bfill );
    printf( "jfill    =%10d\n", jfill );
    printf( "sfill    =%10d\n", sfill );
    printf( "\n%routine          #calls\n===========    ========\n" );
    printf( "%-11s %11d\n", "bump_node", ncalls[0] );
    printf( "%-11s %11d\n", "bump_scard", ncalls[1] );
    printf( "%-11s %11d\n", "join_scard", ncalls[2] );
    printf( "%-11s %11d\n", "hashbackup", ncalls[3] );
    printf( "%-11s %11d\n", "hashscard", ncalls[4] );
    if ( c == 'P' ) {
        printf( "\n d     count\n==    ======\n" );
            for ( d = 1; d <= 4*ntricks; d++ )
                printf( "%2d%10d\n", d, st[d].n );
    }

    exit( 0 );
}
