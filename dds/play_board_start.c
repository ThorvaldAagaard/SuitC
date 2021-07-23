//  name:
//      play_board_start
//
//  purpose:
//      play_board_start starts the analysis of a board
//
//  parameters:
//      none

#include "header.h"

void play_board_start( void )
{
    int           ns[4][5], p = ld, s = trump;
    char          c, in[10];
    static char   hdr[] = "S H D C N";
    static char   *seat[] = { "west", "north", "east", "south" };
    static char   *suit[] = { "spades", "hearts", "diamonds", "clubs", "notrump", "discard" };
    static char   *denom[] = { "S", "H", "D", "C", " N" };

    //  initialise
    d_ab = 1;
    d_start = 1;

    //  determine the result for all combinations of leader and denomination
    for ( ld = 0; ld < 4; ld++ ) {
        for ( trump = 0; trump < 5; trump++ ) {
            if ( ( p == ld || p == unknown ) && ( s == trump || s == discard ) ) {
                nresult = 0;
                target.set = NO;
                lnode[0]->w = ld;
                lnode[0]->nl = 0;
                lnode[0]->nh = ntricks;
                hashbackup( 0 );
                build_tree();
                ns[ld][trump] = lnode[0]->nh;
            }
        }
    }
    ld = p;
    trump = s;
    lnode[0]->w = ld;

    //  print the results
    printf( "\n" );
    print_board( 0 );

    if ( ld == unknown && trump == discard ) {
        printf( "\n%12s%s\n", "", hdr );
        printf( "%11s%2x%2x%2x%2x%2x\n", "", ns[1][0], ns[1][1], ns[1][2], ns[1][3], ns[1][4] );
        printf( " %s%13s%s\n", hdr, "", hdr );
        printf( "%2x%2x%2x%2x%2x%12s%2x%2x%2x%2x%2x\n", ns[0][0], ns[0][1], ns[0][2], ns[0][3], ns[0][4],
                                                    "", ns[2][0], ns[2][1], ns[2][2], ns[2][3], ns[2][4] );
        printf( "%12s%s\n", "", hdr );
        printf( "%11s%2x%2x%2x%2x%2x\n", "", ns[3][0], ns[3][1], ns[3][2], ns[3][3], ns[3][4] );
    }

    else if ( ld == unknown ) {
        printf( "\n%4s%s\n", "", denom[trump] );
        printf( "%4s%2x\n", "", ns[1][trump] );
        printf( "%s%6s%s\n", denom[trump], "", denom[trump] );
        printf( "%2x%6s%2x\n", ns[0][trump], "", ns[2][trump] );
        printf( "\n%4s%s\n", "", denom[trump] );
        printf( "%4s%2x\n", "", ns[3][trump] );
    }

    else if ( trump == discard ) {
        printf( "\n%-9s%s\n", seat[ld], hdr );
        printf( "%8s%2x%2x%2x%2x%2x\n", "", ns[ld][0], ns[ld][1], ns[ld][2], ns[ld][ld], ns[ld][4] );
    }

    //  select the leader
redo:
    if ( ld == unknown ) {
        printf( "\nEnter the leader {w|n|e|s|a|.}: " );
        scanf( "%s", in );
        c = in[0];
        if ( c == 'a' )
            print_stat();
        if ( c == '.' )
            exit( 0 );
        ld = c == 'w' || c == 'W'                         ? west
           : c == 'n' || c == 'N'                         ? north
           : c == 'e' || c == 'E' || c == 'o' || c == 'O' ? east
           : c == 's' || c == 'S' || c == 'z' || c == 'Z' ? south
           : unknown;
        lnode[0]->w = ld;
    }

    //  select the denomination
    if ( trump == discard ) {
        printf( "\nEnter the denomination {s|h|d|c|n|a|.}: " );
        scanf( "%s", in );
        c = in[0];
        if ( c == 'a' )
            print_stat();
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
