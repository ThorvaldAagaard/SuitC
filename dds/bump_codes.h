//  name:
//
//  purpose:
//      bump_node   allocates a new NODE entry
//      bump_scard  allocates a new SCARD entry
//      bump_seqc   allocates a new SEQC entry
//
//  parameters:
//      none
//
//  Copyright 2025, Gerrit A. van der Velde, Leiden

#define bump_node( tnode ) \
    ncount++; \
    ncalls[0]++; \
    tnode = nextnode; \
    if ( ncountmax < ncount ) { \
        ncountmax++; \
        nextnode++; \
        if ( ncountmax == NNODE ) { \
            printf( "No more memory for the NODE entries; increase the value of NNODE\n" ); \
            print_stat( 'P' ); \
        } \
    } \
    else \
        nextnode = nextnode->r; \
    tnode->r = NIL; \
    tnode->d = NIL;

#define bump_scard( tscard ) \
    scount++; \
    ncalls[1]++; \
    tscard = nextscard; \
    if ( scountmax < scount ) { \
        scountmax++; \
        nextscard++; \
        if ( scountmax == NSCARD ) { \
            printf( "No more memory for the SCARD entries; increase the value of NSCARD\n" ); \
            print_stat( 'P' ); \
        } \
    } \
    else \
        nextscard = nextscard->r; \
    tscard->r = NIL;

#define bump_seqc( tseqc ) \
    sqcount++; \
    ncalls[2]++; \
    tseqc = nextseqc; \
    if ( sqcountmax < sqcount ) { \
        sqcountmax++; \
        nextseqc++; \
        if ( sqcountmax == NSEQC ) { \
            printf( "No more memory for the SEQC entries; increase the value of NSEQC\n" ); \
            print_stat( 'P' ); \
        } \
    } \
    else \
        nextseqc = nextseqc->r; \
    tseqc->r = NIL;
