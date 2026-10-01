//  name:
//      print_board
//
//  purpose:
//      print_all_results prints all results
//
//  parameters:
//      d               in      the depth in the NODE tree
//      cards           out     the cards in each hand
//
//  Copyright 2026, Gerrit A. van der Velde, Leiden

#include "header.h"

void print_all_results( int d, COLOR *cards )
{
    int           b, i, k, l, match, n, s;
    static char   hexdigit[] = "0123456789abcdef";
    static char   suits[] = "SHDC-";
    static char   symbols[] = "-123456789TJQKA=";

    //  collect the cards
    for ( s = 0; s < 4; s++ )
        cards[s] = RENONCE;
    for ( l = 0; l < boardcount[d]; l++ ) {
        b = blist[d][l];
        k = res[b].k;
        for ( n = 0; n < k; n++ )
            cards[res[b].s[n]] |= set(res[b].i[n]);
    }

    //  print the suits
    printf( "\n\n\nd=%02d  ", d );
    for ( s = 0; s < 4; s++ )
        for ( i = ace; i >= two; i-- )
            if ( cards[s] & set(i) )
                printf( "%3c", suits[s] );
    printf( "\n      " );

    //  print the cards
    for ( s = 0; s < 4; s++ )
        for ( i = ace; i >= two; i-- )
            if ( cards[s] & set(i) )
                printf( "%3c", symbols[i] );
    printf( "\n      " );

    //  print a separator line
    match = NO;
    for ( s = 0; s < 4; s++ )
        for ( i = ace; i >= two; i-- )
            if ( cards[s] & set(i) )
                { printf( "%s", match ? "---" : "  -" ); match = YES; }
    printf( "\n" );

    //  print the results
    for ( l = 0; l < boardcount[d]; l++ ) {
        b = blist[d][l];
        k = res[b].k;
        printf( "b=%03d ", b+1 );
        for ( s = 0; s < 4; s++ ) {
            for ( i = ace; i >= two; i-- ) {
                if ( cards[s] & set(i) ) {
                    match = NO;
                    for ( n = 0; n < k; n++ ) {
                        if ( s == res[b].s[n] && i == res[b].i[n] ) {
                            match = YES;
                            printf( "%3c", hexdigit[res[b].ns[n]] );
                        }
                    }
                    if ( ! match )
                        printf( "  %c", '-' );
                }
            }
        }
        printf( "\n" );
    }
}
