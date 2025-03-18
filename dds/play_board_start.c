//  name:
//      play_board_start
//
//  purpose:
//      play_board_start starts the analysis of a board
//
//  parameters:
//      none
//
//  Copyright 2025, Gerrit A. van der Velde, Leiden

#include "header.h"

void play_board_start( void )
{
    int           b, l, ns[NBOARDS][4][5], p = ld, s = trump;
    char          c, in[10];
    static char   hdr[] = "S H D C N";
    static char   *seat[] = { "west", "north", "east", "south" };
    static char   *suit[] = { "spades", "hearts", "diamonds", "clubs", "notrump", "discard" };
    static char   *denom[] = { "S", "H", "D", "C", " N" };

    //  initialise
    d_ab = 1;
    d_start = 1;

    //  determine the result for all combinations of leader and denomination
    for ( l = 0; l < boardcount[1]; l++ ) {
        b = blist[1][l];
        for ( ld = 0; ld < 4; ld++ ) {
            for ( trump = 0; trump < 5; trump++ ) {
                if ( ( p == ld || p == unknown ) && ( s == trump || s == discard ) ) {
                    nresult = 0;
                    target.set = NO;
                    wnode[0]->w_p = ld;
                    wnode[0]->nl = 0;
                    wnode[0]->nh = ntricks;
                    hashbackup( 1, b );
                    build_ply( 1, b, ld );
                    build_tree( d_start, b );
                    ns[b][ld][trump] = wnode[0]->nh;
                }
            }
        }
        ld = p;
        trump = s;
        wnode[0]->w_p = ld;
    }

    //  print the boards
    print_all_boards( 0 );

    //  both the leader and the trump suit are unknown
    if ( ld == unknown && trump == discard ) {
        printf( "\n%20s%s\n", "", hdr );
        for ( l = 0; l < boardcount[1]; l++ ) {
            b = blist[1][l];
            printf( "b=%03d%14s%2x%2x%2x%2x%2x\n", b+1, "", ns[b][1][0], ns[b][1][1], ns[b][1][2], ns[b][1][3], ns[b][1][4] );
        }
        printf( "%9s%s%13s%s\n", "", hdr, "", hdr );
        for ( l = 0; l < boardcount[1]; l++ ) {
            b = blist[1][l];
            printf( "b=%03d%3s%2x%2x%2x%2x%2x%12s%2x%2x%2x%2x%2x\n",
                     b+1, "", ns[b][0][0], ns[b][0][1], ns[b][0][2], ns[b][0][3], ns[b][0][4],
                          "", ns[b][2][0], ns[b][2][1], ns[b][2][2], ns[b][2][3], ns[b][2][4] );
        }
        printf( "%20s%s\n", "", hdr );
        for ( l = 0; l < boardcount[1]; l++ ) {
            b = blist[1][l];
            printf( "b=%03d%14s%2x%2x%2x%2x%2x\n", b+1, "", ns[b][3][0], ns[b][3][1], ns[b][3][2], ns[b][3][3], ns[b][3][4] );
        }
    }

    //  the leader is unknown
    else if ( ld == unknown ) {
        printf( "\n%13s%s\n", "", denom[trump] );
        for ( l = 0; l < boardcount[1]; l++ ) {
            b = blist[1][l];
            printf( "b=%03d%7s%2x\n", b+1, "", ns[b][1][trump] );
        }
        printf( "%9s%s%7s%s\n", "", denom[trump], "", denom[trump] );
        for ( l = 0; l < boardcount[1]; l++ ) {
            b = blist[1][l];
            printf( "b=%03d%3s%2x%6s%2x\n", b+1, "", ns[b][0][trump], "", ns[b][2][trump] );
        }
        printf( "%13s%s\n", "", denom[trump] );
        for ( l = 0; l < boardcount[1]; l++ ) {
            b = blist[1][l];
            printf( "b=%03d%7s%2x\n", b+1, "", ns[b][3][trump] );
        }
    }

    //  the trump suit is unknown
    else if ( trump == discard ) {
        printf( "\n%-9s%s\n", seat[ld], hdr );
        for ( l = 0; l < boardcount[1]; l++ ) {
            b = blist[1][l];
            printf( "b=%03d%3s%2x%2x%2x%2x%2x\n", b+1, "", ns[b][ld][0], ns[b][ld][1], ns[b][ld][2], ns[b][ld][3], ns[b][ld][4] );
        }
    }

    //  select the leader
redo:
    if ( ld == unknown ) {
        printf( "\nEnter the leader {w|n|e|s|p|P|.}: " );
        scanf( "%s", in );
        c = in[0];
        if ( c == 'p' || c == 'P' )
            print_stat( c );
        if ( c == '.' )
            exit( 0 );
        ld = c == 'w' || c == 'W'                         ? west
           : c == 'n' || c == 'N'                         ? north
           : c == 'e' || c == 'E' || c == 'o' || c == 'O' ? east
           : c == 's' || c == 'S' || c == 'z' || c == 'Z' ? south
           : unknown;
        wnode[0]->w_p = ld;
    }

    //  select the denomination
    if ( trump == discard ) {
        printf( "\nEnter the denomination {s|h|d|c|n|p|P|.}: " );
        scanf( "%s", in );
        c = in[0];
        if ( c == 'p' || c == 'P' )
            print_stat( c );
        if ( c == '.' )
            exit( 0 );
        trump = c == 's' || c == 'S'                         ? spades
              : c == 'h' || c == 'H'                         ? hearts
              : c == 'd' || c == 'D' || c == 'r' || c == 'R' ? diamonds
              : c == 'c' || c == 'C' || c == 'k' || c == 'K' ? clubs
              : c == 'n' || c == 'N'                         ? notrump
              : discard;
    }

    //  check the input
    if ( ld == unknown || trump == discard )
        goto redo;
}
