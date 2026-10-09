//  name:
//      hashseqc
//
//  purpose:
//      hashseqc enters a SEQC entry in the the SEQC hashtable
//
//  parameters:
//      seqc            in      the root of the SEQC tree
//
//  Copyright 2026, Gerrit A. van der Velde, Leiden

#include "header.h"

struct SEQC *hashseqc( struct SEQC *seqc )
{
    int           match;
    unsigned long inx = 0L;
    struct SEQC   *lseqc, *ltable, *tseqc, zseqc;

    //  return immediately for NIL entries
    if ( ! seqc )
        return( seqc );

    //  loop over the SEQC tree
    lseqc = seqc;
    while ( lseqc ) {
        inx = ( ( inx + (unsigned long) lseqc->n ) * IA + IC ) % IM;
        inx = ( ( inx + (unsigned long) lseqc->p ) * IA + IC ) % IM;
        inx = ( ( inx + (unsigned long) lseqc->c ) * IA + IC ) % IM;
        lseqc = lseqc->d;
    }
    inx = inx % QHASHL;

    //  check the old SEQC hashtable slots
    ltable = seqctable[inx];
    while ( ltable ) {

        //  check the SEQC hashtable slot
        lseqc = seqc;
        tseqc = ltable->d;
        match = YES;
        while ( match && lseqc && tseqc ) {
            match &= lseqc->n == tseqc->n && lseqc->p == tseqc->p && lseqc->c == tseqc->c ;
            lseqc = lseqc->d;
            tseqc = tseqc->d;
        }

        //  we found a match; return
        if ( match && lseqc == NIL && tseqc == NIL )
            return( ltable->d );

        ltable = ltable->r;
    }

    //  no match is found; copy the SEQC list
    lseqc = seqc;
    tseqc = &zseqc;
    tseqc->d = NIL;
    while ( lseqc ) {
        qcount++;
        if ( qcount == NSEQC ) {
            printf( "No more memory for the SEQC entries; increase the value of NSEQC\n" );
            print_stat( 'P' );
        }
        tseqc->d = nextseqc;
        tseqc = tseqc->d;
        nextseqc++;
        tseqc->d = NIL;
        tseqc->r = NIL;
        tseqc->n = lseqc->n;
        tseqc->p = lseqc->p;
        tseqc->c = lseqc->c;
        lseqc = lseqc->d;
    }

    //  insert the SEQC tree in the hashtable
    qcount++;
    qfill++;
    if ( qcount == NSEQC ) {
        printf( "No more memory for the SEQC entries; increase the value of NSEQC\n" );
        print_stat( 'P' );
    }
    if ( 2*qfill >= QHASHL ) {
        printf( "No more memory for the SEQC hashtable; increase the value of QHASHL\n" );
        print_stat( 'P' );
    }
    tseqc = nextseqc;
    nextseqc++;
    tseqc->r = seqctable[inx];
    seqctable[inx] = tseqc;
    tseqc->d = zseqc.d;

    return( zseqc.d );
}
