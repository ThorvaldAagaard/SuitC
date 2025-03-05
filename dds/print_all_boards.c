//  name:
//      print_all_boards
//
//  purpose:
//      print_all_boards prints all boards
//
//  parameters:
//      d               in      the depth in the NODE tree
//
//  Copyright 2025, Gerrit A. van der Velde, Leiden

#include "header.h"

void print_all_boards( int d )
{
    int           b, i, l, n, p, s, t = st[d].t;
    COLOR         allc[4];
    static char   suits[] = "SHDC";
    static char   symbol[] = "-123456789TJQKA";

    //  print the header
    printf( "\n\n\nbrd\n" );
    for ( s = 0; s < 4; s++ )
        allc[s] = RENONCE;
    for ( l = 0; l < boardcount[d]; l++ ) {
        b = blist[d][l];
        for ( s = 0; s < 4; s++ )
            for ( p = 0; p < 4; p++ )
                allc[s] |= brd[t][b][s][p];
    }

    //  print the north hands
    b = blist[d][0];
    printf( "---\t" );
    for ( s = 0; s < 4; s++ ) {
        if ( allc[s] ) {
            n = 0;
            printf( "%c:  ", suits[s] );
            if ( brd[t][b][s][north] ) {
                for ( i = ace; i >= two; i-- ) {
                    if ( set(i) & brd[t][b][s][north] ) {
                        n++;
                        printf( "%c", symbol[i] );
                    }
                }
            }
            else
                printf( "-" );
            if ( n < 4 )
                printf( "\t" );
            printf( "\t\t" );
        }
    }
    printf( "\n" );

    //  print the east and west hands
    for ( l = 0; l < boardcount[d]; l++ ) {

        b = blist[d][l];
        printf( "%03d\t", b+1 );
        for ( s = 0; s < 4; s++ ) {

            //  print the west hands
            if ( allc[s] ) {
                n = 0;
                if ( brd[t][b][s][west] ) {
                    for ( i = ace; i >= two; i-- ) {
                        if ( set(i) & brd[t][b][s][west] ) {
                            n++;
                            printf( "%c", symbol[i] );
                        }
                    }
                }
                else
                    printf( "-" );
                if ( n < 8 )
                    printf( "\t" );
            }

            //  print the east hands
            if ( allc[s] ) {
                n = 0;
                if ( brd[t][b][s][east] ) {
                    for ( i = ace; i >= two; i-- ) {
                        if ( set(i) & brd[t][b][s][east] ) {
                            n++;
                            printf( "%c", symbol[i] );
                        }
                    }
                }
                else
                    printf( "-" );
                if ( n < 8 )
                    printf( "\t" );
                printf( "\t" );
            }
        }

        printf( "\n" );
    }

    //  print the south hands
    b = blist[d][0];
    printf( "\t" );
    for ( s = 0; s < 4; s++ ) {
        if ( allc[s] ) {
            n = 0;
            printf( "    " );
            if ( brd[t][b][s][south] ) {
                for ( i = ace; i >= two; i-- ) {
                    if ( set(i) & brd[t][b][s][south] ) {
                        n++;
                        printf( "%c", symbol[i] );
                    }
                }
            }
            else
                printf( "-" );
            if ( n < 4 )
                printf( "\t" );
            printf( "\t\t" );
        }
    }
    printf( "\n" );
}
