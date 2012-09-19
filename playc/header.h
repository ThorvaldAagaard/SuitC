/* Definitions of types used in program play_suit
 */

#include <stdlib.h>
#include <stdio.h>

typedef enum { two,  three, four, five,  six,  seven, eight,
               nine, ten,   jack, queen, king, ace, pound, dot } CARD;

typedef enum { west, north, east, south, anyplayer } SEATS;

typedef unsigned short COLOR;
typedef COLOR SUIT[4];

typedef struct {
    short ln,           /* limit the number of directional leads from north */
          cn,           /* the number of remaining directional leads from north */
          ls,           /* limit the number of directional leads from south */
          cs;           /* the number of remaining directional leads from south */
} TRICK;

typedef struct {
    int   p,            /* pointer to the parent node */
          b,            /* pointer to the right sibling node */
          s,            /* pointer to the son node */
          i;            /* pointer to the index list of boards */
    COLOR c;            /* the cards played in this ply */
    int   win,          /* winner, only if ( ply%4 == 0 ) */
          who,          /* the player of this node */
          ply,          /* the depth of this node in the tree */
          term:8;
} NODE;

typedef struct {
    SUIT   s;           /* the 4 hands of the board */
    int    d;           /* the multiplicity of the board */
    double p;           /* the a priori percentage for the board */
    char   c[32];       /* cardsymbols */
} BOARD;

typedef struct {
    int north,
        east,
        south,
        west;
    int b;              /* pointer to the sibling entry */
    int node;           /* pointer to the node */
} HASH;

typedef struct {
    int   i:16,         /* index in the board table */
          min:8,        /* minimum number of tricks won by NS */
          max:8,        /* maximum number of tricks won by NS */
          b;            /* pointer to the right sibling index */
} INDEX;

typedef struct {
    int i,              /* index in the board table */
    b;                  /* pointer to the next list entry */
} LIST;

#define ALLCARDS        0x1FFF
#define NCARDS          13
#define TRUE            1
#define YES             1
#define FALSE           0
#define NO              0
#define NIL             0
#define RENONCE         ( (COLOR) 0 )
#define addcard( c )    ( ( (COLOR) 1 ) << ( c ) )
#define discard( s, c ) ( ( s ) ^ ( ( s ) & ( c ) ) )
#define low_c( c )      ( ( c ) & -( c ) )
#define BOARDS          8192
#define HASHL           524288
#define NODES           2000000
#define INDICES         3000000

int   CHECKNODE;        /* print changes in node "CHECKNODE" */
int   DEBUGNODE;        /* print changes in all nodes */
int   KEEPEW;           /* keep EW nodes with equal value */
int   KEEPNS;           /* keep NS nodes with equal value */
int   TRUNCATE;         /* chop tree if no more branching occurs */

int   w_min;            /* the minimum number of west cards on each board */
int   w_max;            /* the minimum number of west cards on each board */
int   w_vac;            /* the number of vacant places in the west hand */
int   e_min;            /* the minimum number of east cards on each board */
int   e_max;            /* the minimum number of east cards on each board */
int   e_vac;            /* the number of vacant places in the east hand */

int   boardcnt;         /* number of EW distributions */
COLOR w_fixed;          /* known cards in the west hand */
COLOR e_fixed;          /* known cards in the east hand */
COLOR hands[4];         /* the initial hand pattern */
int   s_cnt[4];         /* the number of sequences in each hand */
SUIT  s_cards[NCARDS];  /* the sequences in the initial hands */
BOARD board[BOARDS];    /* the set of all possible suit distributions */

int   node_header;      /* head of the NODE tree */
int   next_node;        /* first unused slot in the NODE list */
int   high_node;        /* the highest node used */
NODE  node[NODES];

int   next_index;       /* first unused slot in the INDEX list */
int   high_index;       /* the highest index used */

int   next_hash;        /* first unused slot in the INDEX list */
int   high_hash;        /* the highest index used */
int   hlink[HASHL];
HASH  htable[HASHL];

int   list_header;      /* head of the LIST list */
int   next_list;        /* first unused slot in the LIST list */
LIST  list[256];

int   ntricks;          /* the number of tricks to be played */
TRICK tricks[14];       /* the tricks that so far have been played */
SUIT  played[14];       /* the cards that so far have been played */
SUIT  unplayed[14];     /* the cards that are not yet played */

int   maxply;           /* the maximum depth in the tree */
int   ew_c_count;       /* the number of rows in the board header */
int   b_count[14];      /* the number of entries in each line */
char  b_print[128][14]; /* the board_header */

int   pivot[128];       /* pivots for the minimax procedure */
float a[128][256];      /* payoff matrix for the minimax procedure */
