/* name:
 *      create_all_boards
 *
 * purpose:
 *      create_all_boards create the complete board set.
 *      full board set
 *
 * parameters:
 *      none
 *
 * processing:
 *      step1:  count the number of cards
 *      step2:  create the first board
 *      step3:  create all other boards
 *      step4:  combine boards that differ only by cards of the same rank
 *      step5:  delete boards with too few or too many cards and
 *              scale the percentages
 */

#include "header.h"

int fac( n, r )
int n, r;
{
    int    i, f = 1;
    if ( n < 0 || r < 0 || n < r )
        return 0;
    for( i = 0; i < r; i++ )
        f = ( f * (n-i) ) / (i+1);
    return ( f );
}

int create_all_boards()
{
    int    ic, ii, i, j, jj, k, n, cnt, cnt0, activ[8192];
    COLOR  ewcards, c, cards, ci, cj;
    int    fac();

    /* count the cards */
    ic = 0;
    ewcards = hands[west] | hands[east];
    cards = ewcards;
    while ( cards ) {
        ic++;
        cards ^= low_c(cards);
    }

    /* create the first board */
    boardcnt = 0;
    cnt0 = boardcnt;
    cnt = 0;
    board[cnt0+cnt].s[north] = hands[north];
    board[cnt0+cnt].s[south] = hands[south];
    board[cnt0+cnt].s[east] = RENONCE;
    board[cnt0+cnt].s[west] = ewcards;
    cnt++;
    cnt0 = boardcnt;
    boardcnt += cnt;

    /* create all other boards */
    ii = 0;
    while ( cnt ) {
        ii++;
        cnt = 0;
        cards = ewcards;
        while( cards ) {
            c = low_c( cards );
            for ( i = cnt0; i < boardcnt; i++ ) {
                if ( c > board[i].s[east] ) {
                    board[boardcnt+cnt].s[north] = board[i].s[north];
                    board[boardcnt+cnt].s[south] = board[i].s[south];
                    board[boardcnt+cnt].s[east] = board[i].s[east] | c;
                    board[boardcnt+cnt].s[west] = ewcards ^
                            ( ewcards & board[boardcnt+cnt].s[east] );
                    cnt++;
                }
                else
                    break;
            }
            cards ^= c;
        }
        cnt0 = boardcnt;
        boardcnt += cnt;
    }

    /* combine boards that differ only by cards of the same rank */
    cnt = 0;
    cnt0 = 0;
    determine_sequences( ewcards, hands[north] | hands[south], east );

    for ( k = 0; k <= ic; k++ ) {

        jj = fac( ic, k );
        for ( j = cnt0; j < cnt0+jj; j++ )
            activ[j] = YES;

        for ( n = 0; n < s_cnt[east]; n++ ) {
            for ( j = cnt0+1; j < cnt0+jj; j++ ) {
                if ( activ[j] ) {
                    for ( i = cnt0; i < j; i++ ) {
                        ci = ( s_cards[n][east] & board[i].s[east] ) ^ board[i].s[east];
                        cj = ( s_cards[n][east] & board[j].s[east] ) ^ board[j].s[east];
                        if ( activ[i] && ci == cj ) {
                            activ[j] = NO;
                            break;
                        }
                    }
                }
            }
        }

        for ( j = cnt0; j < cnt0+jj; j++ ) {
            if ( activ[j] ) {
                board[cnt].s[north] = board[j].s[north];
                board[cnt].s[south] = board[j].s[south];
                board[cnt].s[east] = board[j].s[east];
                board[cnt].s[west] = board[j].s[west];
                cnt++;
            }
        }
        cnt0 += jj;
    }
    boardcnt = cnt;

    /* if no boards are left */
    if ( boardcnt == 0 ) {
        printf( "\nERROR: no boards are left!\n\n" );
        exit( 1 );
    }

    return 0;
}
