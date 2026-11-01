//      Copyright 2026, Gerrit A. van der Velde, Leiden

COLOR  brd[14][NBOARDS][4][4];          //  the unplayed cards
double times[2];                        //  the cpu time measured at the start and the end of the program
int    bcount;                          //  the number of occupied slots in the BACKUP hash table
int    blist[NPLIES+2][NBOARDS+1];      //  the list of boards
int    boardcount[NPLIES+1];            //  the number of EW distributions
int    d_ab;                            //  the lowest ply in the alpha-beta pruning
int    d_back;                          //  the ply at which the analysis is restarted
int    d_start;                         //  the start ply for the analysis
int    jcount;                          //  the number of occupied slots in the JOINC hash table
int    ld;                              //  the opening leader
int    ncount;                          //  the number of active NODE entries
int    ncountmax;                       //  the maximum number of active NODE entries
int    nodemax;                         //  the number of slots allocated for the NODE tree
int    nresult;                         //  the number of results
int    ntricks;                         //  the number of tricks that will be played
int    qcount;                          //  the number of SEQC entries
int    qfill;                           //  the number of occupied slots in the SEQC hashtable
int    scount;                          //  the number of active SCARD entries
int    trump;                           //  the trump suit
struct BACKUP *backup[14];              //  the pointers to the BACKUP slots for each trick
struct BACKUP *backuptable[2*NBACKUP+2];//  the BACKUP hash table
struct BACKUP *nextbackup;              //  the first unused slot in the BACKUP hash table
struct JOINC  *joinctable[2*NJOINC+2];  //  the JOINC hash table
struct JOINC  *nextjoinc;               //  the first unused slot in the JOINC hash table
struct NODE   *nextnode;                //  the first unused slot in the NODE tree
struct NODE   *wnode[NPLIES+2];         //  the stack of NODE entries
struct RES    res[NBOARDS];             //  the result of the analysis
struct RESULT result[14];               //  the result of the analysis
struct SCARD  *nextscard;               //  the first unused slot in the SCARD tree
struct SCARD  *scard[14][NBOARDS][4];   //  pointer to the SCARD slots with remaining cards
struct SCARD  *scardtable[2*NSCARD+2];  //  the SCARD hash table
struct SEQC   *nextseqc;                //  the first unused slot in the SEQC list
struct SEQC   *seqctable[QHASHL];       //  the SEQC hash table
struct STACK  st[NPLIES+1];             //  the description of each ply
struct TARGET target;                   //  the target for alpha-beta pruning
