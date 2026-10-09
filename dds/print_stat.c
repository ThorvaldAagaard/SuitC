//  name:
//      print_stat
//
//  purpose:
//      print_stat prints the statistics of an analysis
//
//  parameters:
//      c               in      the print option
//
//  Copyright 2026, Gerrit A. van der Velde, Leiden

#include "header.h"

void print_stat( char c )
{
    int           d;

    times[1] = TICK * clock();
    printf( "\n\nroutine          #calls\n===========    ========\n" );
    printf( "%-11s %11.3f\n", "cpu time", times[1] - times[0] );
    printf( "%-11s %11d\n", "bcount", bcount );
    printf( "%-11s %11d\n", "jcount", jcount );
    printf( "%-11s %11d\n", "ncount", ncountmax );
    printf( "%-11s %11d\n", "qcount", qcount );
    printf( "%-11s %11d\n", "scount", scount );
    printf( "%-11s %11d\n", "qfill", qfill );
    if ( c == 'P' ) {
        printf( "\n d     count\n==    ======\n" );
            for ( d = 1; d <= 4*ntricks; d++ )
                printf( "%2d%10d\n", d, st[d].n );
    }

    exit( 0 );
}
