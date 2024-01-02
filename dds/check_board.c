//  name:
//      check_board
//
//  purpose:
//      check_board checks the input, builds the initial SCARD trees
//      and initialises the root of the NODE tree
//
//  parameters:
//      none
//
//  Copyright 2024, Gerrit A. van der Velde, Leiden

#include "header.h"

void check_board( void )
{
    int           b, i, l, n, p, q, s;

    //  has each player the same number of cards?
    for ( l = 0; l < blist[0].n; l++ ) {
        b = blist[0].b[l];
        for ( p = 0; p < 4; p++ ) {
            n = 0;
            for ( s = 0; s < 4; s++ )
                for ( i = two; i <= ace; i++ )
                    if ( set(i) & brd[b][0][s][p] )
                        n++;
            if ( b == 0 && p == 0 )
                ntricks = n;
            else if ( n != ntricks ) {
                printf( "The number of cards isn't the same for all players\n" );
                exit( 99 );
            }
        }
    }

    //  exit if no cards have been dealt
    if ( ! ntricks ) {
        printf( "No cards have been dealt\n" );
        exit( 99 );
    }

    //  check for duplicate cards
    for ( l = 0; l < blist[0].n; l++ ) {
        b = blist[0].b[l];
        for ( s = 0; s < 4; s++ ) {
            for ( p = 0; p < 4; p++ ) {
                for ( q = p+1; q < 4; q++ ) {
                    if ( brd[b][0][s][p] & brd[b][0][s][q] ) {
                        printf( "The board contains duplicate cards: brd[%02d][0][%d][%d]=%04x and brd[%02d][0][%d][%d]=%04x\n",
                                b, s, p, brd[b][0][s][p] >> 2, b, s, q, brd[b][0][s][q] >> 2 );
                        exit( 99 );
                    }
                }
            }
        }
    }

    //  initialise the root of the NODE tree
    wnode[0]->c = RENONCE;
    wnode[0]->d = NIL;
    wnode[0]->nc = 0;
    wnode[0]->nd = 0;
    wnode[0]->nh = ntricks;
    wnode[0]->nl = 0;
    wnode[0]->p = ld;
    wnode[0]->r = NIL;
    wnode[0]->s = discard;
    wnode[0]->t = ntricks ? NO : YES;
    wnode[0]->w_p = ld;
}
