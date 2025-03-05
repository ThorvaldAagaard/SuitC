//  name:
//      print_scard
//
//  purpose:
//      print_scard prints a SCARD node
//
//  parameters:
//      t               in      the trick that is printed
//      b               in      the board that is analysed
//      s               in      the suit that is reported
//
//  Copyright 2025, Gerrit A. van der Velde, Leiden

#include "header.h"

void print_scard( int t, int b, int s )
{
    int           i, k;
    char          str[14];
    COLOR         *brd;
    static char   seats[] = "WNES";
    static char   suits[] = "SHDC";
    static char   symbols[] = "-123456789TJQKA";
    struct SEQC   *lseqc;

    if ( scard[t][b][s] ) {

        //  print the north hand
        k = 0;
        brd = scard[t][b][s]->brd;
        for ( i = ace+1; i >= 0; i-- )
            if ( set(i) & brd[north] )
                str[k++] = symbols[i];
        str[k] = '\0';
        printf( "\n\t%c: %s%s", suits[s], str, k < 5 ? "\t\t" : "\t" );

        //  print the SCARD sequences
        lseqc = scard[t][b][s]->sq;
        while ( lseqc ) {
            printf( "    %c:%x:", seats[lseqc->p], lseqc->n );
            for ( i = ace+1; i >= 0; i-- )
                if ( set(i) & lseqc->c )
                    printf( "%c", symbols[i] );
            lseqc = lseqc->r;
        }

        //  print the west hand
        k = 0;
        for ( i = ace+1; i >= 0; i-- )
            if ( set(i) & brd[west] )
                str[k++] = symbols[i];
        str[k] = '\0';
        printf( "\n%c: %s%s", suits[s], str, k < 5 ? "\t\t" : "\t" );

        //  print the east hand
        printf( "%c: ", suits[s] );
        for ( i = ace; i >= two; i-- )
            if ( set(i) & brd[east] )
                printf( "%c", symbols[i] );

        //  print the south hand
        printf( "\n\t%c: ", suits[s] );
        for ( i = ace; i >= two; i-- )
            if ( set(i) & brd[south] )
                printf( "%c", symbols[i] );
        printf( "\n" );
    }
}
