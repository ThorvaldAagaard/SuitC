//  name:
//      print_board
//
//  purpose:
//      print_board prints all hands
//
//  parameters:
//      d               in      the depth in the NODE tree
//
//  Copyright 2023, Gerrit A. van der Velde, Leiden

void print_all_boards( int d )
{
    int           h, l, n, nc, s, t = st[d].t;
    static char   suits[] = "SHDC";
    static char   card[] = "-123456789TJQKA";

    //  print the north hands
    printf( "\n" );
    for ( s = 0; s < 4; s++ ) {
        for ( l = 0; l < hlist[d].n; l++ ) {
            h = hlist[d].h[l];
            nc = 21;
            if ( s == 0 )
                printf( "h=%02d    %c: ", h+1, suits[s] );
            else
                printf( "        %c: ", suits[s] );
            for ( n = ace; n >= two; n-- ) {
                if ( set(n) & brd[h][t][s][north] ) {
                    nc--;
                    printf( "%c", card[n] );
                }
            }
            if ( l < hlist[d].n - 1 )
                for ( n = 0; n < nc; n++ )
                    printf( " " );
        }
        printf( "\n" );
    }

    //  print the west and east hands
    for ( s = 0; s < 4; s++ ) {
        for ( l = 0; l < hlist[d].n; l++ ) {
            h = hlist[d].h[l];
            nc = 13;
            printf( "%c: ", suits[s] );
            for ( n = ace; n >= two; n-- ) {
                if ( set(n) & brd[h][t][s][west] ) {
                    nc--;
                    printf( "%c", card[n] );
                }
            }
            for ( n = 0; n < nc; n++ )
                printf( " " );
            nc = 13;
            printf( "%c: ", suits[s] );
            for ( n = ace; n >= two; n-- ) {
                if ( set(n) & brd[h][t][s][east] ) {
                    nc--;
                    printf( "%c", card[n] );
                }
            }
            if ( l < hlist[d].n - 1 )
                for ( n = 0; n < nc; n++ )
                    printf( " " );
        }
        printf( "\n" );
    }

    //  print the south hands
    for ( s = 0; s < 4; s++ ) {
        for ( l = 0; l < hlist[d].n; l++ ) {
            h = hlist[d].h[l];
            nc = 21;
            printf( "        %c: ", suits[s] );
            for ( n = ace; n >= two; n-- ) {
                if ( set(n) & brd[h][t][s][south] ) {
                    nc--;
                    printf( "%c", card[n] );
                }
            }
            if ( l < hlist[d].n - 1 )
                for ( n = 0; n < nc; n++ )
                    printf( " " );
        }
        printf( "\n" );
    }
}
#ifdef SKIP
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
        nc = 13;
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
#endif
