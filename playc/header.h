/* Definitions of types used in program play_suit
 */

#include <stdlib.h>
#include <stdio.h>

typedef enum { west, north, east, south } SEATS;

typedef unsigned short COLOR;
typedef COLOR SUIT[4];

typedef struct {
    SUIT   s;           /* the 4 hands of the board */
} BOARD;

#define NCARDS          13
#define YES             1
#define NO              0
#define RENONCE         ( (COLOR) 0 )
#define addcard( c )    ( ( (COLOR) 1 ) << ( c ) )
#define low_c( c )      ( ( c ) & -( c ) )
#define BOARDS          8192

int   boardcnt;         /* number of EW distributions */
COLOR hands[4];         /* the initial hand pattern */
int   s_cnt[4];         /* the number of sequences in each hand */
SUIT  s_cards[NCARDS];  /* the sequences in the initial hands */
BOARD board[BOARDS];    /* the set of all possible suit distributions */
