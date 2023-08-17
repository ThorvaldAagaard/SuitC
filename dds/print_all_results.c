//  name:
//      print_board
//
//  purpose:
//      print_all_results prints all results
//
//  parameters:
//      d               in      the depth in the NODE tree
//      cards           out     the cards c$in all hands
//
//  Copyright 2023, Gerrit A. van der Velde, Leiden

void print_all_results( int d, COLOR *cards )
{
    int           h, i, k, l, match, n, s;
    static char   hexdigit[] = "0123456789abcdef";
    static char   suits[] = "shdc-";
    static char   symbols[] = "-123456789tjqka=";

    //  collect the cards
    for ( s = 0; s < 4; s++ )
        cards[s] = RENONCE;
    for ( l = 0; l < hlist[d].n; l++ ) {
        h = hlist[d].h[l];
        k = res[h].k;
        for ( n = 0; n < k; n++ )
            cards[res[h].s[n]] |= set(res[h].i[n]);
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
    for ( l = 0; l < hlist[d].n; l++ ) {
        h = hlist[d].h[l];
        k = res[h].k;
        printf( "h=%02d  ", h+1 );
        for ( s = 0; s < 4; s++ ) {
            for ( i = ace; i >= two; i-- ) {
                if ( cards[s] & set(i) ) {
                    match = NO;
                    for ( n = 0; n < k; n++ ) {
                        if ( s == res[h].s[n] && i == res[h].i[n] ) {
                            match = YES;
                            printf( "%3c", hexdigit[res[h].ns[n]] );
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
