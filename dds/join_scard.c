//  name:
//      join_scard
//
//  purpose:
//      join_scard joins adjacent card sequences of the same player
//
//  parameters:
//      d               in      the depth in the tree
//      s               in      the suit that is analysed

//  #include "header.h"

void join_scard( int d, int s )
{
    int           match, p, t = st[d].t;
    unsigned long inx;
    struct JOINC  *ljoinc;
    struct SCARD  *lc, *ltable, *tc, zscard;

    //  locate an existing JOINC slot
    inx = ( (unsigned long) s * IA + IC ) % IM;
    for ( p = 0; p < 4; p++ )
        inx = ( ( inx + (unsigned long) brd[t][s][p] ) * IA + IC ) % IM;
    inx = inx % JHASHL;

    //  check the old JOINC slots
    ncalls[2]++;
    ljoinc = joinctable[inx];
    while ( ljoinc ) {
        match = s == ljoinc->s;
        for ( p = 0; p < 4; p++ )
            match &= brd[t][s][p] == ljoinc->brd[p];
        if ( match ) {
            sc[t+1][s] = ljoinc->sc;
            for ( p = 0; p < 4; p++ )
                brd[t+1][s][p] = ljoinc->outc[p];
            return;
        }
        ljoinc = ljoinc->r;
    }

    //  create a new JOINC slot
    bump_joinc( ljoinc );
    ljoinc->r = joinctable[inx];
    joinctable[inx]= ljoinc;
    ljoinc->s = s;
    for ( p = 0; p < 4; p++ ) {
        ljoinc->brd[p] = brd[t][s][p];
        ljoinc->outc[p] = brd[t][s][p];
    }

    //  convert the cards into a SCARD list
    zscard.d = build_scard( s, brd[t][s] );

    //  locate an existing SCARD slot
    inx = 0;
    ncalls[4]++;
    lc = zscard.d;
    while ( lc ) {
        inx = ( ( inx + (unsigned long) lc->n ) * IA + IC ) % IM;
        inx = ( ( inx + (unsigned long) lc->p ) * IA + IC ) % IM;
        inx = ( ( inx + (unsigned long) lc->s ) * IA + IC ) % IM;
        lc = lc->d;
    }
    inx = inx % SHASHL;

    //  check the old SCARD slots
    ltable = scardtable[inx];
    while ( ltable ) {

        match = YES;
        lc = zscard.d;
        tc = ltable->d;
        while ( lc && tc && match ) {
            match &= lc->n == tc->n;
            match &= lc->p == tc->p;
            match &= lc->s == tc->s;
            lc = lc->d;
            tc = tc->d;
        }

        if ( match && ! lc && ! tc ) {
            ljoinc->sc = ltable->d;
            goto build_joinc;
        }

        ltable = ltable->r;
    }

    //  insert the SCARD entry in the hash tables
    sfill++;
    bump_scard( ltable );
    ltable->d = zscard.d;
    ltable->r = scardtable[inx];
    scardtable[inx] = ltable;
    ljoinc->sc = scardtable[inx]->d;

    //  rebuild the SCARD list
build_joinc:
    if ( zscard.d != ljoinc->sc ) {

        //  delete the old SCARD list
        scount--;
        lc = zscard.d;
        while ( lc->d ) {
            scount--;
            lc->r = lc->d;
            lc = lc->d;
        }
        lc->r = nextscard;
        nextscard = zscard.d;

        //  collect the cards
        lc = ljoinc->sc;
        for ( p = 0; p < 4; p++ )
            ljoinc->outc[p] = RENONCE;
        while ( lc ) {
            ljoinc->outc[lc->p] |= lc->c;
            lc = lc->d;
        }
    }

    //  create the stack entries
    sc[t+1][s] = ljoinc->sc;
    for ( p = 0; p < 4; p++ )
        brd[t+1][s][p] = ljoinc->outc[p];

    //  check the fill factor of the hash table
    if ( ( 2 * sfill >= SHASHL ) ) {
        printf( "ERROR: No more memory available: shashl=%d sfill=%d\n", SHASHL, sfill );
        exit( 99 );
    }

    //  check the fill factor of the hash table
    if ( ( 2 * jfill >= JHASHL ) ) {
        printf( "ERROR: No more memory available: jhashl=%d jfill=%d\n", JHASHL, jfill );
        exit( 99 );
    }
}
