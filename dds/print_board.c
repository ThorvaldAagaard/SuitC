//  name:
//      print_board
//
//  purpose:
//      print_board prints the board
//
//  parameters:
//      t               in      the trick that is printed
//
//  Copyright 2023, Gerrit A. van der Velde, Leiden

void print_board( int t )
{
    int           n, nc, s;
    static char   suits[] = "SHDC";
    static char   card[] = "-123456789TJQKA";

    //  print the north hand
    for ( s = 0; s < 4; s++ ) {
        printf( "        %c: ", suits[s] );
        for ( n = ace; n >= two; n-- )
            if ( set(n) & brd[t][s][north] )
                printf( "%c", card[n] );
        printf( "\n" );
    }

    //  print the west and east hands
    for ( s = 0; s < 4; s++ ) {
        nc = 13;
        printf( "%c: ", suits[s] );
        for ( n = ace; n >= two; n-- ) {
            if ( set(n) & brd[t][s][west] ) {
                nc--;
                printf( "%c", card[n] );
            }
        }
        for ( n = 0; n < nc; n++ )
            printf( " " );
        printf( "%c: ", suits[s] );
        for ( n = ace; n >= two; n-- )
            if ( set(n) & brd[t][s][east] )
                printf( "%c", card[n] );
        printf( "\n" );
    }

    //  print the south hand
    for ( s = 0; s < 4; s++ ) {
        printf( "        %c: ", suits[s] );
        for ( n = ace; n >= two; n-- )
            if ( set(n) & brd[t][s][south] )
                printf( "%c", card[n] );
        printf( "\n" );
    }
}
