//  name:
//      print_board
//
//  purpose:
//      print_board prints all boards
//
//  parameters:
//      d               in      the depth in the NODE tree
//
//  Copyright 2024, Gerrit A. van der Velde, Leiden

#include "header.h"

void print_all_boards( int d )
{
    int           b, l, ll, n, nc, s, t = st[d].t;
    static char   suits[] = "SHDC";
    static char   card[] = "-123456789TJQKA";

    //  print the boards in chunks of 5
    for ( ll = 0; ll < boardcount[d]; ll += 5 ) {

        //  print the north hands
        printf( "\n\n\n" );
        for ( s = 0; s < 4; s++ ) {
            for ( l = ll; l < MIN(ll+5,boardcount[d]); l++ ) {
                b = blist[d][l];
                nc = 21;
                if ( s == 0 )
                    printf( "b=%02d    %c: ", b+1, suits[s] );
                else
                    printf( "        %c: ", suits[s] );
                for ( n = ace; n >= two; n-- ) {
                    if ( set(n) & brd[b][t][s][north] ) {
                        nc--;
                        printf( "%c", card[n] );
                    }
                }
                if ( l < MIN(ll+5,boardcount[d]) - 1 )
                    for ( n = 0; n < nc; n++ )
                        printf( " " );
            }
            printf( "\n" );
        }

        //  print the west and east hands
        for ( s = 0; s < 4; s++ ) {
            for ( l = ll; l < MIN(ll+5,boardcount[d]); l++ ) {
                b = blist[d][l];
                nc = 13;
                printf( "%c: ", suits[s] );
                for ( n = ace; n >= two; n-- ) {
                    if ( set(n) & brd[b][t][s][west] ) {
                        nc--;
                        printf( "%c", card[n] );
                    }
                }
                for ( n = 0; n < nc; n++ )
                    printf( " " );
                nc = 13;
                printf( "%c: ", suits[s] );
                for ( n = ace; n >= two; n-- ) {
                    if ( set(n) & brd[b][t][s][east] ) {
                        nc--;
                        printf( "%c", card[n] );
                    }
                }
                if ( l < MIN(ll+5,boardcount[d]) - 1 )
                    for ( n = 0; n < nc; n++ )
                        printf( " " );
            }
            printf( "\n" );
        }

        //  print the south hands
        for ( s = 0; s < 4; s++ ) {
            for ( l = ll; l < MIN(ll+5,boardcount[d]); l++ ) {
                b = blist[d][l];
                nc = 21;
                printf( "        %c: ", suits[s] );
                for ( n = ace; n >= two; n-- ) {
                    if ( set(n) & brd[b][t][s][south] ) {
                        nc--;
                        printf( "%c", card[n] );
                    }
                }
                if ( l < MIN(ll+5,boardcount[d]) - 1 )
                    for ( n = 0; n < nc; n++ )
                        printf( " " );
            }
            printf( "\n" );
        }
    }
}
