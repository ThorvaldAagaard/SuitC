//      Copyright 2024, Gerrit A. van der Velde, Leiden

COLOR  brd[NBOARDS][NTRICKS+1][5][4];   //  the unplayed cards
double times[2];                        //  the cpu time measured at the start and the end of the program
int    bfill;                           //  the number of occupied slots in the BACKUP hash table
int    d_ab;                            //  the lowest ply in the alpha-beta pruning
int    d_back;                          //  the ply at which the analysis is restarted
int    d_start;                         //  the start ply for the analysis
int    jfill;                           //  the number of occupied slots in the JOINC hash table
int    ld;                              //  the opening leader
int    ncalls[16];                      //  the number of calls of selected routines
int    ncount;                          //  the number of active NODE entries
int    ncountmax;                       //  the maximum number of active NODE entries
int    nresult;                         //  the number of results
int    ntricks;                         //  the number of tricks that will be played
int    scount;                          //  the number of active SCARD entries
int    scountmax;                       //  the maximum number of active SCARD entries
int    sfill;                           //  the number of occupied slots in the SCARD hash table
int    sqcount;                         //  the number of active SEQC entries
int    sqcountmax;                      //  the maximum number of active SEQC entries
int    trump;                           //  the trump suit
struct BACKUP *backup[NTRICKS+1];       //  the pointers to the BACKUP slots for each trick
struct BACKUP *backuptable[BHASHL+100]; //  the BACKUP hash table
struct BACKUP *nextbackup;              //  the first unused slot in the BACKUP hash table
struct BLIST  blist[NBOARDS+1];         //  the list of boards
struct JOINC  *joinctable[JHASHL+100];  //  the JOINC hash table
struct JOINC  *nextjoinc;               //  the first unused slot in the JOINC hash table
struct NODE   *nextnode;                //  the first unused slot in the NODE tree
struct NODE   *wnode[NPLIES+2];         //  the stack of NODE entries
struct RES    res[NBOARDS];             //  the result of the analysis
struct RESULT result[NTRICKS];          //  the result of the analysis
struct SCARD  *nextscard;               //  the first unused slot in the SCARD tree
struct SCARD  *scard[NBOARDS][NTRICKS+1][4]; //  pointer to the SCARD slots with remaining cards
struct SCARD  *scardtable[SHASHL+100];  //  the SCARD hash table
struct SEQC   *nextseqc;                //  the first unused slot in the SEQC list
struct STACK  st[NPLIES+1];             //  the description of each ply
struct TARGET target;                   //  the target for alpha-beta pruning
