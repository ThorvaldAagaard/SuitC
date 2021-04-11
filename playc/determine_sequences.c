/* name:
 *      determine_sequences
 *
 * purpose:
 *      determine_sequences determines all sequences of cards of
 *      equal rank compared to (some combination of) the cards of the
 *      other players
 *
 * parameters:
 *      hand            in      the hand to be compared
 *      ref_hand        in      the combined holding against which the
 *                              comparison is made
 *      p               in      the player for which the comparison is made
 *
 * processing:
 *      step1:  a while loop is executed until all cards have been
 *              assigned to a sequence
 *      step2:  return the number of sequences that have been found
 */

/* #include "header.h"
*/

int determine_sequences( COLOR hand, COLOR ref_hand, int p )
{
    COLOR c, ref_cards, cards, low_ref, lowc;
    int   cnt, nseq;

    /* initialize */
    ref_cards = ref_hand;
    cards = hand;
    s_cnt[p] = 0;
    s_cards[s_cnt[p]][p] = RENONCE;

    /* loop to determine all sequences */
    while( cards ) {
        lowc = low_c( cards );
        while( ref_cards && low_c( ref_cards ) < lowc )
            ref_cards ^= low_c( ref_cards );
        cnt = 0;
        if ( ref_cards ) {
            low_ref = low_c( ref_cards );
            while ( cards && low_c( cards ) < low_ref ) {
                cnt++;
                s_cards[s_cnt[p]][p] |= low_c( cards );
                cards ^= low_c( cards );
            }
        }
        else {
            while ( cards ) {
                cnt++;
                s_cards[s_cnt[p]][p] |= low_c( cards );
                cards ^= low_c( cards );
            }
        }
        s_cnt[p]++;
        s_cards[s_cnt[p]][p] = RENONCE;
    }

    return 0;
}
