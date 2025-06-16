//      Copyright 2024, Gerrit A. van der Velde, Leiden

char   b_print[NCARDS+2][512];          //  the board_header
COLOR  board[4][8192];                  //  the holding for all possible distributions of the east-west cards
COLOR  brd[NCARDS+1][4];                //  the unplayed cards at the start of each trick
COLOR  eastfixed;                       //  the known cards in the east hand
COLOR  h2rule[NCARDS];                  //  the second hand conditions of the third hand rules
COLOR  h3rule[NCARDS];                  //  the rules for the third hand strategy
COLOR  westfixed;                       //  the known cards in the west hand
double a[NNODE][2*NNODE];               //  the matrix with MP or IMP scores
double perc[NBOARDS];                   //  the percentage of the distributions for which the percentage is defined
double prob[8192];                      //  the a priori probabilities for the boards
double times[4];                        //  array with timings
int    boardcount;                      //  the number of EW distributions
int    buildtree;                       //  the type of the NODE tree. 0=truncated tree, 1=full tree, 2=selected tree
int    defence;                         //  lead from east of west in each trick
int    dupl[8192];                      //  the multiplicity of the boards
int    eastvac;                         //  the number of vacant places in the east hand
int    ew_c_count;                      //  the number of rows in the board header
int    ewwinners;                       //  print east-west winners instead of north-south winners
int    foldselect;                      //  1=fold the EW plays, 2=fold the NS plays, 4=select the NS plays
int    icount;                          //  counter for the number of active index entries
int    icountmax;                       //  the maximum number of active index entries
int    leader;                          //  the openings leader at trick one
int    matchpoint;                      //  generate the matchpoint plays
int    matchpointtime;                  //  print the matchpoint analysis time
int    maxnodes;                        //  the maximum number of nodes used for the MP or IMP evaluation
int    maxns;                           //  the maximum number of ns_tricks
int    maxprint;                        //  the maximum number of tricks to be printed
int    minns;                           //  the minimum number of ns_tricks
int    nbest_plays;                     //  the number of elements in array best_play
int    ncards[4];                       //  the maximum number of cards in each hand
int    ncardsmin[4];                    //  the minimum number of cards in each hand
int    ncount;                          //  counter for the number of active NODE entries
int    ncountmax;                       //  the maximum number of active NODE entries
int    nfree[NCARDS+1][4];              //  the number of remaining directional leads
int    nplays;                          //  number of possible plays
int    nruff[NCARDS+1][4];              //  the number of remaining ruffs
int    nrules;                          //  the number of rules for the 3d hand strategy
int    nslimits[2][NBOARDS];            //  the lower and upper bound for the number tricks for north-south (ewwinners==NO) or east-west (ewwinners==YES) on each board
int    nstricks[NBOARDS];               //  the number of tricks for north-south (ewwinners==NO) or east-west (ewwinners==YES) on each board
int    ntricks;                         //  the number of tricks to be played
int    opening_lead;                    //  the card that is lead at trick one
int    pointscore[NCARDS+1];            //  the pointscore for each number of tricks
int    printallmatchpoint;              //  print the MP or IMP results for all plays
int    printallresults;                 //  print a full report of all results
int    printboards;                     //  print the complete board set
int    printfulltree;                   //  print the full NODE tree
int    printhighcard;                   //  print the the highest card of a north-south card sequence in the game tree
int    printoption;                     //  printoption for the game trees (0=English lower, 1=Dutch lower, 2=English upper, 3=Dutch upper)\n" );
int    printscard;                      //  print different card symbols for each card in a card sequence in the board header
int    printsummary;                    //  print a summary of the results
int    printtrunctree;                  //  print the truncated NODE tree
int    qdcount;                         //  the number of active QUIRKD entries
int    qdcountmax;                      //  the maximum number of active QUIRKD entries
int    qhcount;                         //  the number of active QUIRKH entries
int    qhcountmax;                      //  the maximum number of active QUIRKH entries
int    reorder;                         //  flag to indicate that Jeroen Warmerdam's order of the boards is used
int    resultsonly;                     //  report only the results, no plays
int    trace;                           //  trace options: 1=timing, 2=minimax progress
int    tricklimit;                      //  limit on the number of tricks that are played
int    usesuitentries;                  //  maximize the use of entries in the suit itself
int    westvac;                         //  the number of vacant places in the west hand
int    z_count;                         //  the maximum number cards in the west hand
int    zermelo;                         //  1: delete weakly dominated plays in evaluate_mp_results, 2: delete weakly dominated plays in check_zermelo
long   cutoff;                          //  limit on the product of the number of quirks while merging
struct INDEX  *nextindex;               //  the first unused slot in the INDEX list
struct NODE   *nextnode;                //  the first unused slot in the NODE list
struct NODE   *tnode[NPLIES+1];         //  the stack of target NODE entries
struct NODE   *wnode[NPLIES+1];         //  the stack of NODE entries
struct QUIRKD *nextquirkd;              //  the first unused slot in the QUIRKD list
struct QUIRKD quirkdl[NPLIES+1];        //  pointers to the root of QUIRK trees
struct QUIRKH *nextquirkh;              //  the first unused slot in the QUIRKH list
struct RESULT best_play[32];            //  the plays that give the best results for MAX and N<n>
struct RESULT best_result;              //  the best result of all plays
struct SCARD  scards[NCARDS+1][4];      //  the SCARD sequences for each trick
struct SCORE  *score;                   //  the score of each play
