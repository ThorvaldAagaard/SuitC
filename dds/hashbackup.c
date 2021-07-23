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
        if ( ! ( brd[t][s][0] | brd[t][s][1] | brd[t][s][2] | brd[t][s][3] ) ) {
            sc[t+1][s] = NIL;
            for ( p = 0; p < 4; p++ )
                brd[t+1][s][p] = RENONCE;
        }
        else
            join_scard( d, s );
    }

    //  locate an existing BACKUP slot
    ncalls[3]++;
    inx = ( (unsigned long) lnode[d]->w * IA + IC ) % IM;
    inx = ( ( inx + (unsigned long) trump ) * IA + IC ) % IM;
    for ( s = 0; s < 4; s++ )
        inx = ( ( inx + (unsigned long) sc[t+1][s] ) * IA + IC ) % IM;
    inx = inx % BHASHL;

    //  check the old BACKUP slots
    lbackup = backuptable[inx];
    while ( lbackup ) {
        match = lnode[d]->w == lbackup->ld && trump == lbackup->ts;
        for ( s = 0; s < 4; s++ )
            match &= sc[t+1][s] == lbackup->sc[s];
        if ( match ) {
            st[d].backup = lbackup;
            lnode[d]->nh = lbackup->nh;
            lnode[d]->nl = lbackup->nl;
            lnode[d]->d = lbackup->dnode;
            lnode[d]->t = lbackup->t;
            return( YES );
        }
        lbackup = lbackup->r;
    }

    //  build the NODE tree for the next trick
    build_trick( d+1, lnode[d]->w );

    //  create a new BACKUP slot
    bump_backup( lbackup );
    lbackup->r = backuptable[inx];
    backuptable[inx] = lbackup;
    st[d].backup = lbackup;
    lbackup->ld = lnode[d]->w;
    lbackup->ts = trump;
    for ( s = 0; s < 4; s++ )
        lbackup->sc[s] = sc[t+1][s];

    //  check the fill factor of the hash table
    if ( ( 2 * bfill >= BHASHL ) ) {
        printf( "ERROR: No more memory available: bhashl=%d bfill=%d\n", BHASHL, bfill );
        exit( 99 );
    }

    return( NO );
}
