//  name:
//      print_scard
//
//  purpose:
//      print_scard prints a SCARD node
//
//  parameters:
//      t               in      the trick that is printed
//      s               in      the suit that is reported
//
//  Copyright 2024, Gerrit A. van der Velde, Leiden

#include "header.h"

void print_scard( int t, int s, struct SCARD *scard[4][4] )
{
    int           i, k, n, nc;
    char          str[14];
    COLOR         *brd;
    static char   card[] = "-123456789TJQKA";
    static char   seats[] = "wnes";
    static char   suits[] = "SHDC";
    static char   symbols[] = "-123456789tjqka=";
    struct SEQC   *lseqc;

    //  print the north hand
    printf( "\n" );
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
        printf( "         " );

        lseqc = scard[t][s]->sq;
        while ( lseqc ) {
            k = 0;
            for ( i = ace+1; i >= 0; i-- )
                if ( set(i) & lseqc->c )
                    str[k++] = symbols[i];
            str[k] = '\0';
            if ( k < 6 )
                printf( "%c:%x:%-6s", seats[lseqc->p], lseqc->n, str );
            else
                printf( "%c:%x:%s ", seats[lseqc->p], lseqc->n, str );
            lseqc = lseqc->r;
        }
        printf( "\n" );
    }

    //  print the west and east hands
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

    //  print the south hand
    if ( scard[t][s] ) {

        brd = scard[t][s]->brd;
        printf( "        %c: ", suits[s] );
        for ( n = ace; n >= two; n-- )
            if ( set(n) & brd[south] )
                printf( "%c", card[n] );
        printf( "\n" );
    }
}
