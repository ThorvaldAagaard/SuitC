#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

//  Copyright 2025, Gerrit A. van der Velde, Leiden

//  list of cpp defines
//  ----------------------------------------------------------------------------
#define EW( p )                         ( ( p == west ) || ( p == east ) )
#define IA                              (unsigned long) 2416
#define IC                              (unsigned long) 374441
#define IM                              (unsigned long) 1771875
#define low_c( c )                      ( ( c ) & -( c ) )
#define MAX( i, j )                     ( ( i ) > ( j ) ? ( i ) : ( j ) )
#define MIN( i, j )                     ( ( i ) < ( j ) ? ( i ) : ( j ) )
#define NBACKUP                         60000000L
#define NBOARDS                         128
#define NIL                             (void *) 0
#define NJOINC                          1000000L
#define NNODE                           1600000000L
#define NO                              0
#define NPLIES                          52
#define NS( p )                         ( ( p == north ) || ( p == south ) )
#define NSCARD                          100000L
#define NSEQC                           1000000L
#define RENONCE                         ( (COLOR) 0 )
#define set( i )                        ( 1 << i )
#define YES                             1

//  list of typedef declarations
//  ----------------------------------------------------------------------------
typedef unsigned short COLOR;
typedef enum { west, north, east, south, unknown } SEATS;
typedef enum { spades, hearts, diamonds, clubs, notrump, discard, ruff } SUITS;
typedef enum { renonce, one, two, three, four, five, six, seven, eight, nine, ten, jack, queen, king, ace } CARD;

//  list of type declarations
//  ----------------------------------------------------------------------------

struct BACKUP {
    struct BACKUP *r;                   //  pointer to the right sibling node
    struct NODE   *node;                //  the root of the backup NODE tree
    struct SCARD  *scard[4];            //  pointer to the SCARD trees
    unsigned int  ld:2,                 //  the leader to the trick
                  nh:4,                 //  the backup value of the maximum number of north-south tricks
                  nl:4,                 //  the backup value of the minimum number of north-south tricks
                  t:1,                  //  indicator for terminal nodes
                  trump:3;              //  the trump suit
};

struct JOINC {
    struct JOINC  *r;                   //  pointer to the right sibling node
    struct SCARD  *sc;                  //  pointer to the output SCARD tree
    COLOR         brd[4];               //  the cards of each player
    unsigned int  s;                    //  the suit
};

struct NODE {
    struct NODE   *d,                   //  pointer to the oldest son node
                  *r;                   //  pointer to the right sibling node
    COLOR         c,                    //  the cards that are played
                  w_c;                  //  the winning card of the current trick
    unsigned int  nc:1,                 //  1 if north-south win the current trick
                  nd:4,                 //  the number of north-south tricks made in previous plies
                  nh:4,                 //  the maximum number of north-south tricks
                  nl:4,                 //  the minimum number of north-south tricks
                  p:2,                  //  the player of the cards
                  rnk:6,                //  the ranking of the play
                  s:2,                  //  the suit that is played
                  t:1,                  //  the indicator for terminal nodes
                  w_p:2,                //  the winning player of the current trick
                  w_s:2;                //  the winning suit of the current trick
};

struct RES {
    int           k,                    //  the number of results
                  i[14],                //  the card index
                  ns[14],               //  the number north-south tricks
                  s[14];                //  the suit
};

struct RESULT {
    COLOR         c;                    //  the cards that are lead
    unsigned int  s,                    //  the suit that is lead
                  ns;                   //  the result
};

struct SCARD {
    struct SCARD  *r;                   //  pointer to the right sibling
    struct SEQC   *seqc;                //  the list of SEQC sequences
    COLOR         brd[4];               //  the cards of each player
    unsigned int  s;                    //  the suit of the sequence
};

struct SEQC {
    struct SEQC   *r;                   //  pointer to the right sibling
    COLOR         c;                    //  the cards in the sequence
    unsigned int  n,                    //  the number of cards in the sequence
                  p;                    //  the player of the sequence
};

struct STACK {
    unsigned int  l,                    //  the lowest ply in this trick
                  la,                   //  YES if this is the last player to this trick
                  ld,                   //  YES if the lead is made in this ply
                  n,                    //  the number of plays that have been investigated at each depth in the tree
                  t;                    //  the trick number
};

struct TARGET {
    unsigned int  d,                    //  the ply number of the target root
                  p,                    //  the player that plays at the target root
                  ns,                   //  the number of north-south tricks
                  set;                  //  indicator that alpha-beta pruning is active
};

//  list of include files
//  ----------------------------------------------------------------------------
#include "extern.h"

//  list of routines
//  ----------------------------------------------------------------------------
void           bottomup_tree( int d, int b );
void           build_node( int d, int b, int s, int p, COLOR c );
void           build_ply( int d, int b, int p );
void           build_scard( int d, int b, int s, int lookup );
void           build_tree( int d, int b );
void           check_board( void );
int            dds( int argc, char **argv );
struct NODE   *delete_tree( int d_start, struct NODE *root );
int            hashbackup( int d, int b );
void           join_scard( int d, int b, int s );
void           order_plays( int d );
void           play_board( int d, int p );
void           play_board_start( void );
void           print_all_boards( int d );
void           print_all_results( int d, COLOR *cards );
void           print_board( COLOR brd[4][4] );
void           print_node( int d, struct NODE *root, char *ident );
void           print_scard( struct SCARD *lscard );
void           print_stat( char c );
void           print_tree( int d_start, struct NODE *root, char *ident );
void           process_args( int argc, char **argv );
