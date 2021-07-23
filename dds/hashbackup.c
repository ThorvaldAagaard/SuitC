//  name:
//      hashbackup
//
//  purpose:
//      hashbackup determines the index of a BACKUP entry in the hash table
//      the routine returns YES if an existing entry is found in
//      backuptable and NO otherwise.
//
//  parameters:
//      d               in      the depth in the tree

//  #include "header.h"

int hashbackup( int d )
{
    int           match, p, s, t = st[d].t;
    unsigned long inx;
    struct BACKUP *lbackup;

    //  create the JOINC slots
    for ( s = 0; s < 4; s++ ) {
        if ( brd[t-1][s][0] | brd[t-1][s][1] | brd[t-1][s][2] | brd[t-1][s][3] )
            join_scard( d, s );
        else {
            scard[t][s] = NIL;
            for ( p = 0; p < 4; p++ )
                brd[t][s][p] = RENONCE;
        }
    }

    //  locate an existing BACKUP slot
    ncalls[3]++;
    inx = ( (unsigned long) lnode[d-1]->w * IA + IC ) % IM;
    inx = ( ( inx + (unsigned long) trump ) * IA + IC ) % IM;
    for ( s = 0; s < 4; s++ )
        inx = ( ( inx + (unsigned long) scard[t][s] ) * IA + IC ) % IM;
    inx = inx % BHASHL;

    //  check the old BACKUP slots
    lbackup = backuptable[inx];
    while ( lbackup ) {
        match = lnode[d-1]->w == lbackup->ld && trump == lbackup->ts;
        for ( s = 0; s < 4; s++ )
            match &= scard[t][s] == lbackup->sc[s];
        if ( match ) {
            backup[t-1] = lbackup;
            lnode[d-1]->nh = lbackup->nh;
            lnode[d-1]->nl = lbackup->nl;
            lnode[d-1]->d = lbackup->dnode;
            lnode[d-1]->t = lbackup->t;
            return( YES );
        }
        lbackup = lbackup->r;
    }

    //  build the NODE tree for the next trick
    build_trick( d, lnode[d-1]->w );

    //  create a new BACKUP slot
    bump_backup( lbackup );
    lbackup->r = backuptable[inx];
    backuptable[inx] = lbackup;
    backup[t-1] = lbackup;
    lbackup->ld = lnode[d-1]->w;
    lbackup->ts = trump;
    for ( s = 0; s < 4; s++ )
        lbackup->sc[s] = scard[t][s];

    //  check the fill factor of the hash table
    if ( ( 2 * bfill >= BHASHL ) ) {
        printf( "ERROR: No more memory available: bhashl=%d bfill=%d\n", BHASHL, bfill );
        exit( 99 );
    }

    return( NO );
}
