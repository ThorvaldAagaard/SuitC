//  name:
//      hashbackup
//
//  purpose:
//      hashbackup determines the index of a BACKUP entry in the hash table
//      the routine returns YES if an existing entry is found in
//      backuptable and NO otherwise.
//
//  parameters:
//      d               in      the depth in the NODE tree
//      brd             in      the board that is analysed
//      scard           out     the SCARD sequences that are build
//
//  Copyright 2023, Gerrit A. van der Velde, Leiden

int hashbackup( int d, COLOR brd[14][5][4], struct SCARD *scard[14][4] )
{
    int           match, p, s, t = st[d].t;
    unsigned long inx;
    struct BACKUP *lbackup;

    //  create the JOINC slots
    for ( s = 0; s < 4; s++ ) {
        if ( brd[t-1][s][0] | brd[t-1][s][1] | brd[t-1][s][2] | brd[t-1][s][3] )
            join_scard( d, s, brd, scard );
        else {
            scard[t][s] = NIL;
            for ( p = 0; p < 4; p++ )
                brd[t][s][p] = RENONCE;
        }
    }

    //  locate an existing BACKUP slot
    ncalls[3]++;
    inx = ( (unsigned long) lnode[d-1]->w_p * IA + IC ) % IM;
    inx = ( ( inx + (unsigned long) trump ) * IA + IC ) % IM;
    for ( s = 0; s < 4; s++ )
        inx = ( ( inx + (unsigned long) scard[t][s] ) * IA + IC ) % IM;
    inx = inx % BHASHL;

    //  check the old BACKUP slots
    lbackup = backuptable[inx];
    while ( lbackup ) {
        match = lnode[d-1]->w_p == lbackup->ld && trump == lbackup->trump;
        for ( s = 0; s < 4; s++ )
            match &= scard[t][s] == lbackup->sc[s];
        if ( match ) {
            backup[t-1] = lbackup;
            lnode[d-1]->nh = lbackup->nh;
            lnode[d-1]->nl = lbackup->nl;
            lnode[d-1]->d = lbackup->node;
            lnode[d-1]->t = lbackup->t;
            return( YES );
        }
        lbackup = lbackup->r;
    }

    //  build the NODE tree for the next trick
    build_ply( d, lnode[d-1]->w_p, brd, scard );

    //  create a new BACKUP slot
    bfill++;
    lbackup = nextbackup;
    nextbackup++;
    lbackup->r = backuptable[inx];
    backuptable[inx] = lbackup;
    backup[t-1] = lbackup;
    lbackup->ld = lnode[d-1]->w_p;
    lbackup->trump = trump;
    for ( s = 0; s < 4; s++ )
        lbackup->sc[s] = scard[t][s];

    //  check the fill factor of the hash table
    if ( ( 2 * bfill >= BHASHL ) ) {
        printf( "ERROR: No more memory available: bhashl=%d bfill=%d\n", BHASHL, bfill );
        exit( 99 );
    }

    return( NO );
}
