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
    int           b, h, i, k, l, lds, n, ns, q, s, start, t = st[d].t;
    char          c, in[10], out[14];
    COLOR         cards[4];
    static char   seats[] = "wnes";
    static char   suits[] = "shdc-";
    static char   *suit[] = { "spades", "hearts", "diamonds", "clubs", "notrump", "discard" };
    static char   symbols[] = "-123456789tjqka=";
    struct NODE   *bnode[53], znode;
    struct SCARD  *lc, zscard;

    if ( st[d].ld ) {
        for ( l = 0; l < hlist[d].n; l++ ) {
            h = hlist[d].h[l];
            for ( s = 0; s < 4; s++ )
                build_scard( d, s, brd[h], scard[h] );
        }
    }

redo:
    d_back = d;
    d_ab = d+1;
    d_start = d;
    for ( l = 0; l < hlist[d].n; l++ ) {
        h = hlist[d].h[l];
        nresult = 0;
        lnode[d-1]->t = NO;
        lnode[d-1]->nl = 0;
        lnode[d-1]->nh = ntricks - d/4;
        build_ply( d, p, brd[h], scard[h] );
        build_tree( d, brd[h], scard[h] );

        //  collect the results
        k = 0;
        for ( s = 0; s < 4; s++ ) {
            for ( i = ace; i >= two; i-- ) {
                for ( n = 0; n < nresult; n++ ) {
                    if ( result[n].s == s && result[n].c & set(i) ) {
                        res[h].i[k] = i;
                        res[h].ns[k] = result[n].ns;
                        res[h].s[k] = s;
                        k++;
                    }
                }
            }
        }
        res[h].k = k;
    }

    //  print the results
    print_all_boards( d );
    print_all_results( d, cards );
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
    h = 0;
    start = YES;
    lds = st[d].ld ? discard : lnode[st[d].l]->s;
    printf( "%s", "\nSelect the play " );
    for ( s = 0; s < 4; s++ ) {
        if ( cards[s] && ( st[d].ld || s == lds || ! cards[lds] ) ) {
            l = 0;
            for ( i = ace; i >= two; i-- )
                if ( cards[s] & set(i) )
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
    printf( "%s", "|b<n>|p|P|.}: " );

    //  select the play
    scanf( "%s", in );
    c = in[0];
    if ( c == 'p' || c == 'P' )
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
    #ifdef SKIP
    else if ( cards[s] && ( st[d].ld || s == lds || ! brd[h][t][lds][p] ) && brd[h][t][s][p] & set(i) ) ;
    else {
        printf( "The selected play is incorrect\n" );
        goto redo;
    }
    #endif
    else {
        hlist[d+1].n = 0;
        for ( l = 0; l < hlist[d].n; l++ ) {
            h = hlist[d].h[l];
            if ( brd[h][t][s][p] && ( st[d].ld || s == lds || ! brd[h][t][lds][p] ) && brd[h][t][s][p] & set(i) ) {
                hlist[d+1].h[hlist[d+1].n] = h;
                hlist[d+1].n++;
            }
        }
    }
    if ( hlist[d+1].n == 0 ) {
        printf( "The selected play is incorrect\n" );
        goto redo;
    }

    znode.r = NIL;
    lnode[d] = &znode;
    bnode[d] = lnode[d-1]->d;
    h = hlist[d+1].h[0];
    build_node( d, s, p, set(i), brd[h] );
    lnode[d-1]->d = lnode[d];
    lnode[d]->nd = lnode[d-1]->nd + lnode[d-1]->nc;
    for ( l = 0; l < hlist[d].n; l++ ) {
        h = hlist[d].h[l];
        brd[h][t][s][p] ^= brd[h][t][s][p] & set(i);
    }

    //  play the next card
    if ( d < 4*ntricks ) {
        q = st[d].la ? lnode[d]->w_p : (p+1)%4;
        play_board( d+1, q );
        for ( l = 0; l < hlist[d].n; l++ ) {
            h = hlist[d].h[l];
            brd[h][t][s][p] = scard[h][t][s]->brd[p];
        }
    }

    //  We are finished
    else {

        lnode[d-1]->d = bnode[d];
        for ( l = 0; l < hlist[d].n; l++ ) {
            h = hlist[d].h[l];
            brd[h][t][s][p] = scard[h][t][s]->brd[p];
        }
        ns = lnode[d]->nd + lnode[d]->nc;
        printf( "\nd=%02d    ns=%x ew=%x   %s\n", d, ns, st[d].t - ns, suit[trump] );
        printf( "\n%s", "We are finished. What next? {b<n>|p|P|.}: " );
        scanf( "%s", in );
        c = in[0];
        if ( c == 'p' || c == 'P' )
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
            if ( d_back < d ) {
                lnode[d_back-1]->d = bnode[d_back];
                return;
            }
            else {
                printf( "Forward backtracking isn't allowed\n" );
                goto redo;
            }
        }
    }

    if ( d == d_back )
        goto redo;
}
