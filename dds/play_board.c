//  name:
//      play_board
//
//  purpose:
//      play_board builds the NODE tree for a ply
//
//  parameters:
//      d               in      the depth in the NODE tree
//
//  Copyright 2023, Gerrit A. van der Velde, Leiden

void play_board( int d, int p )
{
    int           b, i, k, l, lds, n, ns, q, s, start, t = st[d].t;
    char          c, in[10], out[14];
    static char   seats[] = "wnes";
    static char   suits[] = "shdc-";
    static char   *suit[] = { "spades", "hearts", "diamonds", "clubs", "notrump", "discard" };
    static char   symbols[] = "-123456789tjqka=";
    struct NODE   znode;
    struct SCARD  *lc, zscard;
    struct { int i, ns, s; } res[13];

    if ( st[d].ld )
        for ( s = 0; s < 4; s++ )
            build_scard( d, s );

redo:
    d_back = d;
    d_ab = d+1;
    d_start = d;
    nresult = 0;
    lnode[d-1]->nl = 0;
    lnode[d-1]->nh = ntricks - d/4;
    build_ply( d, p );
    build_tree( d );

    //  collect the results
    k = 0;
    for ( s = 0; s < 4; s++ ) {
        for ( i = ace; i >= two; i-- ) {
            for ( n = 0; n < nresult; n++ ) {
                if ( result[n].s == s && result[n].c & set(i) ) {
                    res[k].i = i;
                    res[k].ns = result[n].ns;
                    res[k].s = s;
                    k++;
                }
            }
        }
    }

    //  print the results
    printf( "\n" ); print_board( t );
    printf( "\nd=%02d", d );
    printf( "  " ); for ( n = 0; n < k; n++ ) printf( "  %c", suits[res[n].s] ); printf( "\n    " );
    printf( "  " ); for ( n = 0; n < k; n++ ) printf( "  %c", symbols[res[n].i] ); printf( "\n    " );
    printf( "  " ); for ( n = 0; n < k; n++ ) printf( "  %x", res[n].ns ); printf( "\n" );

    ns = lnode[st[d].l-1]->nd + lnode[st[d].l-1]->nc;
    printf( "\nd=%02d    ns=%x ew=%x   %s", d, ns, st[st[d].l-1].t - ns, suit[trump] );
    for ( b = st[d].l; b < d; b++ ) {
        for ( i = two; i <= ace; i++ )
            if ( lnode[b]->c & set(i) )
                break;
        printf( "   %c %c %c", seats[lnode[b]->p], suits[lnode[b]->s], symbols[i] );
    }
    printf( "   %c ? ?\n", seats[p] );

    //  build the request string
    start = YES;
    lds = st[d].ld ? discard : lnode[st[d].l]->s;
    printf( "%s", "\nSelect the play " );
    for ( s = 0; s < 4; s++ ) {
        if ( brd[t][s][p] && ( st[d].ld || s == lds || ! brd[t][lds][p] ) ) {
            l = 0;
            for ( i = ace; i >= two; i-- )
                if ( brd[t][s][p] & set(i) )
                    out[l++] = symbols[i];
            out[l] = '\0';
            if ( start && l ) {
                printf( "{%c:%s", suits[s], out );
                start = NO;
            }
            else if ( l )
                printf( "|%c:%s", suits[s], out );
        }
    }
    printf( "%s", "|b<n>|s|S|.}: " );

    //  select the play
    scanf( "%s", in );
    c = in[0];
    if ( c == 's' || c == 'S' )
        print_stat( c );
    if ( c == '.' )
        exit( 0 );

    //  handle backtracking
    if ( c == 'b' ) {

        if ( in[1] == '\0' )
            d_back = d-1;
        else {
            d_back = in[1] - '0';
            if ( in[2] != '\0' )
                d_back = 10 * d_back + in[2] - '0';
        }

        if ( d_back < d )
            return;
        else {
            printf( "Forward backtracking isn't allowed\n" );
            goto redo;
        }
    }

    //  select the play
    s = c == 's' || c == 'S'                         ? spades
      : c == 'h' || c == 'H'                         ? hearts
      : c == 'd' || c == 'D' || c == 'r' || c == 'R' ? diamonds
      : c == 'c' || c == 'C' || c == 'k' || c == 'K' ? clubs
      : discard;
    c = in[1];
    i = c == 'A' || c == 'a'                         ? 14
      : c == 'K' || c == 'k' || c == 'H' || c == 'h' ? 13
      : c == 'Q' || c == 'q' || c == 'V' || c == 'v' ? 12
      : c == 'J' || c == 'j' || c == 'B' || c == 'b' ? 11
      : c == 'T' || c == 't' || c == '1'             ? 10
      : c - '0';

    //  verify the correctness of the play
    if ( s == discard || i < 2 || i > 14 ) {
        printf( "The selected play is incorrect\n" );
        goto redo;
    }
    else if ( brd[t][s][p] && ( st[d].ld || s == lds || ! brd[t][lds][p] ) && brd[t][s][p] & set(i) ) ;
    else {
        printf( "The selected play is incorrect\n" );
        goto redo;
    }

    znode.r = NIL;
    lnode[d] = &znode;
    build_node( d, s, p, set(i) );
    lnode[d-1]->d = lnode[d];
    lnode[d]->nd = lnode[d-1]->nd + lnode[d-1]->nc;
    brd[t][s][p] ^= brd[t][s][p] & set(i);

    //  play the next card
    if ( d < 4*ntricks ) {
        q = st[d].la ? lnode[d]->w_p : (p+1)%4;
        play_board( d+1, q );
        brd[t][s][p] = scard[t][s]->brd[p];
    }

    //  We are finished
    else {

        brd[t][s][p] = scard[t][s]->brd[p];
        ns = lnode[d]->nd + lnode[d]->nc;
        printf( "\nd=%02d    ns=%x ew=%x   %s\n", d, ns, st[d].t - ns, suit[trump] );
        printf( "\n%s", "We are finished. What next? {b<n>|s|S|.}: " );
        scanf( "%s", in );
        c = in[0];
        if ( c == 's' || c == 'S' )
            print_stat( c );
        if ( c == '.' )
            exit( 0 );

        //  handle backtracking
        if ( c == 'b' ) {
            if ( in[1] == '\0' )
                d_back = d-1;
            else {
                d_back = in[1] - '0';
                if ( in[2] != '\0' )
                    d_back = 10 * d_back + in[2] - '0';
            }
            if ( d_back < d )
                return;
            else {
                printf( "Forward backtracking isn't allowed\n" );
                goto redo;
            }
        }
    }

    if ( d == d_back )
        goto redo;
}
