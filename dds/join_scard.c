//  name:
//      join_scard
//
//  purpose:
//      join_scard joins adjacent card sequences of the same player
//
//  parameters:
//      d               in      the depth in the NODE tree
//      s               in      the suit that is analysed
//
//  Copyright 2022, Gerrit A. van der Velde, Leiden

//  #include "header.h"

void join_scard( int d, int s )
{
    int           match, n, p, t = st[d].t;
    unsigned long inx;
    struct JOINC  *ljoinc;
    struct SCARD  *ltable;
    struct SEQC   *sq, *tq;

    //  locate an existing JOINC slot
    inx = ( (unsigned long) s * IA + IC ) % IM;
    for ( p = 0; p < 4; p++ )
        inx = ( ( inx + (unsigned long) brd[t-1][s][p] ) * IA + IC ) % IM;
    inx = inx % JHASHL;

    //  check the old JOINC slots
    ncalls[2]++;
    ljoinc = joinctable[inx];
    while ( ljoinc ) {
        match = s == ljoinc->s;
        for ( p = 0; p < 4; p++ )
            match &= brd[t-1][s][p] == ljoinc->brd[p];
        if ( match ) {
            scard[t][s] = ljoinc->sc;
            for ( p = 0; p < 4; p++ )
                brd[t][s][p] = scard[t][s]->brd[p];
            return;
        }
        ljoinc = ljoinc->r;
    }

    //  create a new JOINC slot
    jfill++;
    ljoinc = nextjoinc;
    nextjoinc++;
    ljoinc->r = joinctable[inx];
    joinctable[inx] = ljoinc;
    ljoinc->s = s;
    for ( p = 0; p < 4; p++ )
        ljoinc->brd[p] = brd[t-1][s][p];

    //  convert the cards into a SCARD list
    build_scard( d, s );

    //  locate an existing SCARD slot
    ncalls[4]++;
    sq = scard[t][s]->sq;
    inx = ( (unsigned long) scard[t][s]->s * IA + IC ) % IM;
    for ( n = 0; n < scard[t][s]->n; n++ ) {
        inx = ( ( inx + (unsigned long) sq[n].n ) * IA + IC ) % IM;
        inx = ( ( inx + (unsigned long) sq[n].p ) * IA + IC ) % IM;
    }
    inx = inx % SHASHL;

    //  check the old SCARD slots
    ltable = scardtable[inx];
    while ( ltable ) {

        sq = scard[t][s]->sq;
        tq = ltable->sq;
        match = YES;
        if ( scard[t][s]->n == ltable->n ) {

            for ( n = 0; n < scard[t][s]->n; n++ ) {
                match &= sq[n].n == tq[n].n;
                match &= sq[n].p == tq[n].p;
            }

            //  we found an old SCARD slot
            if ( match ) {
                scount--;
                scard[t][s]->r = nextscard;
                nextscard = scard[t][s];
                scard[t][s] = ltable;
                ljoinc->sc = ltable;
                for ( p = 0; p < 4; p++ )
                    brd[t][s][p] = ltable->brd[p];
                return;
            }
        }

        ltable = ltable->r;
    }

    //  insert the SCARD entry in the hash tables
    sfill++;
    scard[t][s]->r = scardtable[inx];
    scardtable[inx] = scard[t][s];
    ljoinc->sc = scard[t][s];

    //  check the fill factor of the hash table
    if ( 2 * sfill >= SHASHL ) {
        printf( "ERROR: No more memory available: shashl=%d sfill=%d\n", SHASHL, sfill );
        exit( 99 );
    }

    //  check the fill factor of the hash table
    if ( 2 * jfill >= JHASHL ) {
        printf( "ERROR: No more memory available: jhashl=%d jfill=%d\n", JHASHL, jfill );
        exit( 99 );
    }
}
