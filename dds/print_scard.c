//  name:
//      print_scard
//
//  purpose:
//      print_scard prints a SCARD node
//
//  parameters:
//      t               in      the trick that is printed

//  #include "header.h"

void print_scard( int t )
{
    int           i, k, n, nc, s;
    COLOR         *brd;
    char          str[14];
    static char   card[] = "-123456789TJQKA";
    static char   seats[] = "wnes";
    static char   suits[] = "SHDC";
    static char   symbols[] = "-123456789tjqka=";

    //  print the north hand
    printf( "\n" );
    for ( s = 0; s < 4; s++ ) {

        if ( scard[t][s] ) {

            nc = 13;
            brd = scard[t][s]->brd;
            printf( "        %c: ", suits[s] );
            for ( n = ace; n >= two; n-- ) {
                if ( set(n) & brd[north] ) {
                    printf( "%c", card[n] );
                    nc--;
                }
            }
            for ( n = 0; n < nc; n++ )
                printf( " " );
            printf( "         \n" );
        }
    }

    //  print the west and east hands
    for ( s = 0; s < 4; s++ ) {

        if ( scard[t][s] ) {

            nc = 13;
            brd = scard[t][s]->brd;
            printf( "%c: ", suits[s] );
            for ( n = ace; n >= two; n-- ) {
                if ( set(n) & brd[west] ) {
                    nc--;
                    printf( "%c", card[n] );
                }
            }

            for ( n = 0; n < nc; n++ )
                printf( " " );
            printf( "%c: ", suits[s] );
            for ( n = ace; n >= two; n-- )
                if ( set(n) & brd[east] )
                    printf( "%c", card[n] );
            printf( "\n" );
        }
    }

    //  print the south hand
    for ( s = 0; s < 4; s++ ) {

        if ( scard[t][s] ) {

            brd = scard[t][s]->brd;
            printf( "        %c: ", suits[s] );
            for ( n = ace; n >= two; n-- )
                if ( set(n) & brd[south] )
                    printf( "%c", card[n] );
            printf( "\n" );
        }
    }
}
