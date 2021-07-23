//  name:
//      check_board
//
//  purpose:
//      check_board checks the input, builds the initial SCARD trees
//      and initialises the root of the NODE tree
//
//  parameters:
//      none

#include "header.h"

void check_board( void )
{
    int           n, p, q, s;

    //  count the cards
    for ( p = 0; p < 4; p++ ) {
        for ( s = 0; s < 6; s++ )
            nc[s][p] = 0;
        for ( s = 0; s < 4; s++ )
            for ( n = two; n <= ace; n++ )
                if ( set(n) & brd[0][s][p] )
                    nc[s][p]++;
    }

    //  has each player the same number of cards?
    for ( p = 0; p < 4; p++ ) {
        n = 0;
        for ( s = 0; s < 4; s++ )
            n += nc[s][p];
        if ( p == 0 )
            ntricks = n;
        else if ( n != ntricks ) {
            printf( "The number of cards isn't the same for all players\n" );
            exit( 99 );
        }
    }

    //  exit if no cards have been dealt
    if ( ! ntricks ) {
        printf( "No cards have been dealt\n" );
        exit( 99 );
    }

    //  check for duplicate cards
    for ( s = 0; s < 4; s++ ) {
        for ( p = 0; p < 4; p++ ) {
            for ( q = p+1; q < 4; q++ ) {
                if ( brd[0][s][p] & brd[0][s][q] ) {
                    printf( "The hand contains duplicate cards: c[%d][%d]=%04x and  c[%d][%d]=%04x\n", s, p, brd[0][s][p] >> 2, s, q, brd[0][s][q] >> 2 );
                    exit( 99 );
                }
            }
        }
    }

    //  initialise the root of the NODE tree
    lnode[0]->c = RENONCE;
    lnode[0]->d = NIL;
    lnode[0]->nc = 0;
    lnode[0]->nd = 0;
    lnode[0]->nh = ntricks;
    lnode[0]->nl = 0;
    lnode[0]->p = ld;
    lnode[0]->r = NIL;
    lnode[0]->s = discard;
    lnode[0]->t = ntricks ? NO : YES;
    lnode[0]->w = ld;
}
