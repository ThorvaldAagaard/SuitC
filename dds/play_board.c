//  name:
//      play_board
//
//  purpose:
//      play_board builds the NODE tree for a ply
//
//  parameters:
//      d               in      the depth in the NODE tree
//      p               in      the player of this ply
//
//  Copyright 2026, Gerrit A. van der Velde, Leiden

#include "header.h"

void play_board( int d, int p )
{
    int           a, b, i, k, l, lds, n, ns, q, s, start, t = st[d].t;
    char          c, in[14], out[14];
    COLOR         cards[4], playc;
    static char   seats[] = "WNES";
    static char   suits[] = "SHDC-";
    static char   *suit[] = { "spades", "hearts", "diamonds", "clubs", "notrump", "discard" };
    static char   symbols[] = "-123456789TJQKA=";
    struct NODE   *bnode[NPLIES+1], znode;
    struct SEQC   *lseqc, zseqc;

    if ( st[d].ld ) {
        for ( l = 0; l < boardcount[d]; l++ ) {
            b = blist[d][l];
            for ( s = 0; s < 4; s++ ) {
                scount++;
                zseqc.d = build_seqc( brd[t-1][b][s] );
                scard[t][b][s] = nextscard;
                nextscard++;
                scard[t][b][s]->seqc = zseqc.d;
                for ( q = 0; q < 4; q++ ) {
                    scard[t][b][s]->brd[q] = brd[t-1][b][s][q];
                    brd[t][b][s][q] = scard[t][b][s]->brd[q];
                }
            }
        }
    }

redo:
    d_back = d;
    d_ab = d+1;
    d_start = d;
    for ( l = 0; l < boardcount[d]; l++ ) {
        b = blist[d][l];
        nresult = 0;
        wnode[d-1]->t = NO;
        wnode[d-1]->nl = 0;
        wnode[d-1]->nh = ntricks - d/4;
        build_ply( d, b, p );
        build_tree( d, b );

        //  collect the results
        k = 0;
        for ( s = 0; s < 4; s++ ) {
            for ( i = ace; i >= two; i-- ) {
                for ( n = 0; n < nresult; n++ ) {
                    if ( result[n].s == s && result[n].c & set(i) ) {
                        res[b].i[k] = i;
                        res[b].ns[k] = result[n].ns;
                        res[b].s[k] = s;
                        k++;
                    }
                }
            }
        }
        res[b].k = k;
    }

    //  print the results
    print_all_boards( d );
    print_all_results( d, cards );
    ns = wnode[st[d].l-1]->nd + wnode[st[d].l-1]->nc;
    printf( "\nd=%02d    ns=%x ew=%x   %s", d, ns, st[st[d].l-1].t - ns, suit[trump] );
    for ( a = st[d].l; a < d; a++ ) {
        l = 0;
        for ( i = ace; i >= two; i-- )
            if ( wnode[a]->c & set(i) )
                out[l++] = symbols[i];
        out[l] = '\0';
        printf( "   %c %c %s", seats[wnode[a]->p], suits[wnode[a]->s], out );
    }
    printf( "   %c ? ?\n", seats[p] );

    //  build the request string
    b = 0;
    start = YES;
    lds = st[d].ld ? discard : wnode[st[d].l]->s;
    printf( "%s", "\nSelect the play " );
    for ( s = 0; s < 4; s++ ) {
        k = 0;
        for ( i = ace; i >= two; i-- ) {
            for ( l = 0; l < boardcount[d]; l++ ) {
                b = blist[d][l];
                if ( ( st[d].ld || s == lds || brd[t][b][lds][p] == RENONCE ) && brd[t][b][s][p] & set(i) ) {
                    out[k++] = symbols[i];
                    break;
                }
            }
        }
        out[k] = '\0';
        if ( start && k ) {
            printf( "{%c:%s", suits[s], out );
            start = NO;
        }
        else if ( k )
            printf( "|%c:%s", suits[s], out );
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

    //  select the suit
    s = c == 's' || c == 'S'                         ? spades
      : c == 'h' || c == 'H'                         ? hearts
      : c == 'd' || c == 'D' || c == 'r' || c == 'R' ? diamonds
      : c == 'c' || c == 'C' || c == 'k' || c == 'K' ? clubs
      : discard;

    //  select the cards
    playc = RENONCE;
    for ( k = 1; k <= 13; k++ ) {
        if ( ! in[k] ) break;
        c = in[k];
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
        playc |= set(i);
    }
    l = 0;
    for ( i = ace; i >= two; i-- )
        if ( playc & set(i) )
            out[l++] = symbols[i];
    out[l] = '\0';

    //  select the boards
    boardcount[d+1] = 0;
    for ( l = 0; l < boardcount[d]; l++ ) {
        b = blist[d][l];
        if ( brd[t][b][s][p] && ( st[d].ld || s == lds || ! brd[t][b][lds][p] ) && brd[t][b][s][p] & playc ) {
            blist[d+1][boardcount[d+1]] = b;
            boardcount[d+1]++;
        }
    }
    if ( boardcount[d+1] == 0 ) {
        printf( "The selected play is incorrect\n" );
        goto redo;
    }

    znode.r = NIL;
    wnode[d] = &znode;
    bnode[d] = wnode[d-1]->d;
    b = blist[d+1][0];
    build_node( d, b, s, p, playc );
    wnode[d-1]->d = wnode[d];
    wnode[d]->nd = wnode[d-1]->nd + wnode[d-1]->nc;
    for ( l = 0; l < boardcount[d]; l++ ) {
        b = blist[d][l];
        brd[t][b][s][p] ^= low_c( scard[t][b][s]->brd[p] & playc );
    }

    //  play the next card
    if ( d < 4*ntricks ) {
        q = st[d].la ? wnode[d]->w_p : (p+1)%4;
        play_board( d+1, q );
        for ( l = 0; l < boardcount[d]; l++ ) {
            b = blist[d][l];
            brd[t][b][s][p] = scard[t][b][s]->brd[p];
        }
    }

    //  We are finished
    else {

        for ( l = 0; l < boardcount[d]; l++ ) {
            b = blist[d][l];
            brd[t][b][s][p] = scard[t][b][s]->brd[p];
        }
        ns = wnode[d]->nd + wnode[d]->nc;
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
                wnode[d_back-1]->d = bnode[d_back];
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
