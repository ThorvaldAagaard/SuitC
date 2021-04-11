/* name:
 *      print_boards
 *
 * purpose:
 *      print_boards prints the information about the suit combination
 *
 * parameters:
 *      none
 *
 * processing:
 *      step1:  print the cards in the four hands
 *      step2:  print additional information
 *      step3:  print the full set of boards
 */

/* #include "header.h"
*/

int print_boards()
{
    int i, j, k, n, p;
    char out[14];
    static char symbols[] = "23456789abcde";

    /* print the full set of boards */
    for ( i = 0; i < boardcnt; i++ ) {
        for ( n = 0; n < 3; n++ ) {
            p = 2;
            p = ( n == 1 ? 0 : p );
            p = ( n == 2 ? 1 : p );
            k = 0;
            out[0] = '.';
            out[1] = '\0';
            for ( j = NCARDS-1; j >= 0; j-- ) {
                if ( addcard( j ) & board[i].s[p] ) {
                    out[k] = symbols[j];
                    out[++k] = '\0';
                }
            }
            printf( "%s ", out );
        }
        printf( "\n" );
    }
    printf( "\n" );

    return 0;
}
