//  name:
//      print_scard
//
//  purpose:
//      print_scard prints a SCARD node
//
//  parameters:
//      lscard          in      the SCARD variable to be printed
//
//  Copyright 2025, Gerrit A. van der Velde, Leiden

#include "header.h"

void print_scard( struct SCARD *lscard )
{
    int           i, k;
    char          str[14];
    COLOR         *brd;
    static char   seats[] = "WNES";
    static char   suits[] = "SHDC";
    static char   symbols[] = "-123456789TJQKA";
    struct SEQC   *lseqc;

    if ( lscard ) {

        //  print the north hand
        k = 0;
        brd = lscard->brd;
        for ( i = ace+1; i >= 0; i-- )
            if ( set(i) & brd[north] )
                str[k++] = symbols[i];
        str[k] = '\0';
        printf( "\n\t%c: %s%s", '-', str, k < 5 ? "\t\t" : "\t" );

        //  print the SCARD sequences
        lseqc = lscard->seqc;
        while ( lseqc ) {
            printf( "    %c:%x:", seats[lseqc->p], lseqc->n );
            for ( i = ace+1; i >= 0; i-- )
                if ( set(i) & lseqc->c )
                    printf( "%c", symbols[i] );
            lseqc = lseqc->d;
        }

        //  print the west hand
        k = 0;
        for ( i = ace+1; i >= 0; i-- )
            if ( set(i) & brd[west] )
                str[k++] = symbols[i];
        str[k] = '\0';
        printf( "\n%c: %s%s", '-', str, k < 5 ? "\t\t" : "\t" );

        //  print the east hand
        printf( "%c: ", '-' );
        for ( i = ace; i >= two; i-- )
            if ( set(i) & brd[east] )
                printf( "%c", symbols[i] );

        //  print the south hand
        printf( "\n\t%c: ", '-' );
        for ( i = ace; i >= two; i-- )
            if ( set(i) & brd[south] )
                printf( "%c", symbols[i] );
        printf( "\n" );
    }
}
