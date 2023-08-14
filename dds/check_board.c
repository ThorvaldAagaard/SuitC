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
//  Copyright 2023, Gerrit A. van der Velde, Leiden

void check_board( void )
{
    int           h, i, n, p, q, s;

    //  has each player the same number of cards?
    for ( h = 0; h < nhands; h++ ) {
        for ( p = 0; p < 4; p++ ) {
            n = 0;
            for ( s = 0; s < 4; s++ )
                for ( i = two; i <= ace; i++ )
                    if ( set(i) & brd[h][0][s][p] )
                        n++;
            if ( h == 0 && p == 0 )
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
    for ( h = 0; h < nhands; h++ ) {
        for ( s = 0; s < 4; s++ ) {
            for ( p = 0; p < 4; p++ ) {
                for ( q = p+1; q < 4; q++ ) {
                    if ( brd[h][0][s][p] & brd[h][0][s][q] ) {
                        printf( "The hand contains duplicate cards: brd[%02d][0][%d][%d]=%04x and brd[%02d][0][%d][%d]=%04x\n",
                                h, s, p, brd[h][0][s][p] >> 2, h, s, q, brd[h][0][s][q] >> 2 );
                        exit( 99 );
                    }
                }
            }
        }
    }

    //  initialise the root of the NODE tree
    h = 0;
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
    lnode[0]->w_p = ld;

    printf( "\n#\n" );
}
