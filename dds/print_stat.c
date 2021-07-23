//  name:
//      print_stat
//
//  purpose:
//      print_stat prints the statistics of an analysis
//
//  parameters:
//      none

#include "header.h"

void print_stat( void )
{
    int           d;

    times[1] = TICK * clock();
    printf( "\ncpu time =%9.3f\n", times[1] - times[0] );
    printf( "ncount   =%9d\n", ncountmax );
    printf( "scount   =%9d\n", scountmax );
    printf( "bfill    =%9d\n", bfill );
    printf( "jfill    =%9d\n", jfill );
    printf( "sfill    =%9d\n", sfill );
    printf( "\n%routine         #calls\n===========   ========\n" );
    printf( "%-11s %10d\n", "bump_node", ncalls[0] );
    printf( "%-11s %10d\n", "bump_scard", ncalls[1] );
    printf( "%-11s %10d\n", "join_scard", ncalls[2] );
    printf( "%-11s %10d\n", "hashbackup", ncalls[3] );
    printf( "%-11s %10d\n", "hashscard", ncalls[4] );
    printf( "\n d    count\n==   ======\n" );
    for ( d = 1; d <= 4*ntricks; d++ )
        printf( "%2d %8d\n", d, st[d].n );

    exit( 0 );
}
