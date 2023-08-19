#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

//  Copyright 2023, Gerrit A. van der Velde, Leiden

//  list of cpp defines
//  ----------------------------------------------------------------------------
#define BHASHL                          60000000L
#define EW( p )                         ( ( p == west ) || ( p == east ) )
#define IA                              (unsigned long) 2416
#define IC                              (unsigned long) 374441
#define IM                              (unsigned long) 1771875
#define JHASHL                          300000L
#define low_c( c )                      ( ( c ) & -( c ) )
#define MAX( i, j )                     ( ( i ) > ( j ) ? ( i ) : ( j ) )
#define MIN( i, j )                     ( ( i ) < ( j ) ? ( i ) : ( j ) )
#define NBOARDS                         64
#define NIL                             (void *) 0
#define NNODE                           600000000L
#define NO                              0
#define NS( p )                         ( ( p == north ) || ( p == south ) )
#define NSCARD                          100000L
#define RENONCE                         ( (COLOR) 0 )
#define set( i )                        ( 1 << i )
#define SHASHL                          40000L
#define YES                             1

//  list of typedef's and type declarations
//  ----------------------------------------------------------------------------
typedef unsigned short COLOR;
typedef enum { west, north, east, south, unknown } SEAT;
typedef enum { spades, hearts, diamonds, clubs, notrump, discard } SUITS;
typedef enum { renonce, one, two, three, four, five, six, seven, eight, nine, ten, jack, queen, king, ace } CARD;

struct BACKUP {
    struct BACKUP *r;                   //  pointer to the right sibling node
    struct NODE   *node;                //  the root of the backup NODE tree
    struct SCARD  *sc[4];               //  pointer to the SCARD trees
    unsigned int  ld:2,                 //  the leader to the trick
                  nh:4,                 //  the backup value of the maximum number of north-south tricks
                  nl:4,                 //  the backup value of the minimum number of north-south tricks
                  t:1,                  //  indicator for terminal nodes
                  trump:3;              //  the trump suit
};

struct BLIST {
    int           n,                    //  the number of boards for each ply
                  b[NBOARDS];           //  the board indices
};

struct JOINC {
    struct JOINC  *r;                   //  pointer to the right sibling node
    struct SCARD  *sc;                  //  pointer to the output SCARD tree
    COLOR         brd[4];               //  the cards of each player
    unsigned int  s:4;                  //  the suit
};

struct NODE {
    struct NODE   *d,                   //  pointer to the oldest son node
                  *r;                   //  pointer to the right sibling node
    COLOR         c,                    //  the cards that are played
                  w_c:16;               //  the winning card of the current trick
    unsigned int  nc:1,                 //  1 if north-south win the current trick
                  nd:4,                 //  the number of north-south tricks made in previous plies
                  nh:4,                 //  the maximum number of north-south tricks
                  nl:4,                 //  the minimum number of north-south tricks
                  p:2,                  //  the player of the cards
                  rnk:6,                //  the ranking of the play
                  s:3,                  //  the suit that is played
                  t:1,                  //  the indicator for terminal nodes
                  w_p:3,                //  the winning player of the current trick
                  w_s:3;                //  the winning suit of the current trick
};

struct SEQC {
    COLOR         c;                    //  the cards in the sequence
    unsigned int  n:4,                  //  the number of cards in the sequence
                  p:4;                  //  the player of the sequence
};

struct SCARD {
    struct SCARD  *r;                   //  pointer to the right sibling
    struct SEQC   sq[13];               //  the card sequences
    COLOR         brd[4];               //  the cards of each player
    unsigned int  n:4,                  //  the number of used elements in sq
                  s:4;                  //  the suit of the sequence
};

//  list of variables
//  ----------------------------------------------------------------------------
COLOR  brd[NBOARDS][14][5][4];           //  the unplayed cards
double times[2];                        //  the cpu time measured at the start and the end of the program
int    bfill;                           //  the number of occupied slots in the BACKUP hash table
int    d_ab;                            //  the lowest ply in the alpha-beta pruning
int    d_back;                          //  the ply at which the analysis is restarted
int    d_start;                         //  the start ply for the analysis
int    jfill;                           //  the number of occupied slots in the JOINC hash table
int    ld;                              //  the opening leader
int    ncalls[5];                       //  the number of calls of selected routines
int    ncount;                          //  the number of active NODE entries
int    ncountmax;                       //  the maximum number of active NODE entries
int    nresult;                         //  the number of results
int    ntricks;                         //  the number of tricks that will be played
int    scount;                          //  the number of active SCARD entries
int    scountmax;                       //  the maximum number of active SCARD entries
int    sfill;                           //  the number of occupied slots in the SCARD hash table
int    trump;                           //  the trump suit
struct BACKUP *backup[14];              //  the pointers to the BACKUP slots for each trick
struct BACKUP *backuptable[BHASHL+100]; //  the BACKUP hash table
struct BACKUP *nextbackup;              //  the first unused slot in the BACKUP hash table
struct BLIST  blist[53];                //  the list of boards
struct JOINC  *joinctable[JHASHL+100];  //  the JOINC hash table
struct JOINC  *nextjoinc;               //  the first unused slot in the JOINC hash table
struct NODE   *lnode[54];               //  the stack of NODE entries
struct NODE   *nextnode;                //  the first unused slot in the NODE tree
struct SCARD  *nextscard;               //  the first unused slot in the SCARD tree
struct SCARD  *scard[NBOARDS][14][4];   //  pointer to the SCARD slots with remaining cards
struct SCARD  *scardtable[SHASHL+100];  //  the SCARD hash table

struct RES {
    int           k,                    //  the number of results
                  i[13],                //  the card index
                  ns[13],               //  the number north-south tricks
                  s[13];                //  the suit
} res[NBOARDS];

struct RESULT {
    COLOR         c;                    //  the cards that are lead
    unsigned int  s,                    //  the suit that is lead
                  ns;                   //  the result
} result[13];

struct STACK {
    unsigned int  l,                    //  the lowest ply in this trick
                  la,                   //  YES if this is the last player to this trick
                  ld,                   //  YES if the lead is made in this ply
                  n,                    //  the number of plays that have been investigated at each depth in the tree
                  t;                    //  the trick number
} st[53];                               //  the description of each ply

struct TARGET {
    unsigned int  d,                    //  the ply number of the target root
                  p,                    //  the player that plays at the target root
                  ns,                   //  the number of north-south tricks
                  set;                  //  indicator that alpha-beta pruning is active
} target;

//  list of routines
//  ----------------------------------------------------------------------------
void           bottomup_tree( int d, COLOR brd[14][5][4], struct SCARD *scard[14][4] );
void           build_node( int d, int s, int p, COLOR c, COLOR brd[14][5][4] );
void           build_ply( int d, int p, COLOR brd[14][5][4], struct SCARD *scard[14][4] );
void           build_scard( int d, int s, COLOR brd[14][5][4], struct SCARD *scard[14][4] );
void           build_tree( int d, COLOR brd[14][5][4], struct SCARD *scard[14][4] );
void           check_board( void );
int            dds( int argc, char **argv );
struct NODE   *delete_tree( int d_start, struct NODE *root );
int            hashbackup( int d, COLOR brd[14][5][4], struct SCARD *scard[14][4] );
void           join_scard( int d, int s, COLOR brd[14][5][4], struct SCARD *scard[14][4] );
void           order_plays( int d );
void           play_board( int d, int p );
void           play_board_start( void );
void           print_all_boards( int d );
void           print_all_results( int d, COLOR *cards );
void           print_board( COLOR brd[5][4] );
void           print_node( int d, struct NODE *root, char *ident );
void           print_scard( int t, int s, struct SCARD *scard[4][4] );
void           print_stat( char c );
void           print_tree( int d_start, struct NODE *root, char *ident );
void           process_args( int argc, char **argv );
