//  name:
//      print_board
//
//  purpose:
//      print_board prints the board
//
//  parameters:
//      brd             in      the board that is printed
//
//  Copyright 2024, Gerrit A. van der Velde, Leiden

#include "header.h"

void print_board( COLOR brd[5][4] )
{
    int           n, nc, s;
    static char   suits[] = "SHDC";
    static char   card[] = "-123456789TJQKA";

    //  print the north hand
    for ( s = 0; s < 4; s++ ) {
        printf( "        %c: ", suits[s] );
        for ( n = ace; n >= two; n-- )
            if ( set(n) & brd[s][north] )
                printf( "%c", card[n] );
        printf( "\n" );
    }

    //  print the west and east hands
    for ( s = 0; s < 4; s++ ) {
        nc = NTRICKS;
        printf( "%c: ", suits[s] );
        for ( n = ace; n >= two; n-- ) {
            if ( set(n) & brd[s][west] ) {
                nc--;
                printf( "%c", card[n] );
            }
        }
        for ( n = 0; n < nc; n++ )
            printf( " " );
        printf( "%c: ", suits[s] );
        for ( n = ace; n >= two; n-- )
            if ( set(n) & brd[s][east] )
                printf( "%c", card[n] );
        printf( "\n" );
    }

    //  print the south hand
    for ( s = 0; s < 4; s++ ) {
        printf( "        %c: ", suits[s] );
        for ( n = ace; n >= two; n-- )
            if ( set(n) & brd[s][south] )
                printf( "%c", card[n] );
        printf( "\n" );
    }
}
