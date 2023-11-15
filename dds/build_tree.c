//  name:
//      build_tree
//
//  purpose:
//      build_tree builds the complete NODE tree
//
//  parameters:
//      d               in      the depth in the NODE tree
//      b               in      the board that is analysed
//
//  Copyright 2023, Gerrit A. van der Velde, Leiden

void build_tree( int d, int b )
{
    int           nh, nl, p, s, t = st[d].t;
    struct BACKUP zbackup;

    //  loop over the nodes
    wnode[d] = wnode[d-1]->d;
    while ( wnode[d] ) {

        //  play the cards and prepare for the next ply
        st[d].n++;
        p = wnode[d]->p;
        s = wnode[d]->s;
        wnode[d]->nd = wnode[d-1]->nd + wnode[d-1]->nc;
        brd[b][t][s][p] ^= low_c( brd[b][t][s][p] & wnode[d]->c );
        if ( st[d].la && d < 4*ntricks ) {

            //  check the alpha-beta pruning
            nh = wnode[d]->nd + wnode[d]->nc + wnode[d]->nh;
            nl = wnode[d]->nd + wnode[d]->nc + wnode[d]->nl;
            if ( target.set && NS(target.p) && ( nh <= target.ns || target.ns <  nl ) ||
                 target.set && EW(target.p) && ( nh <  target.ns || target.ns <= nl ) )
                backup[t] = &zbackup;

            //  check the BACKUP hash table
            else if ( hashbackup( d+1, b ) ) {

                //  check the alpha-beta pruning
                nh = wnode[d]->nd + wnode[d]->nc + wnode[d]->nh;
                nl = wnode[d]->nd + wnode[d]->nc + wnode[d]->nl;
                if ( target.set && NS(target.p) && ( nh <= target.ns || target.ns <  nl ) ||
                     target.set && EW(target.p) && ( nh <  target.ns || target.ns <= nl ) )
                    backup[t] = &zbackup;

                //  the NODE tree contains still branches
                else if ( wnode[d]->d )
                    build_tree( d+1, b );
            }

            //  no backup NODE tree exists
            else
                build_tree( d+1, b );
        }

        //  the NODE tree is fully analysed
        else if ( st[d].la || wnode[d]->t ) ;

        //  the NODE tree contains still branches
        else if ( wnode[d]->d )
            build_tree( d+1, b );

        //  build the next ply of the NODE tree
        else {
            build_ply( d+1, b, (p+1)%4 );
            build_tree( d+1, b );
        }

        //  backup the NODE tree
        bottomup_tree( d, brd[b], scard[b] );
    }
}
