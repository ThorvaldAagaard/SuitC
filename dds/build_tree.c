//  name:
//      build_tree
//
//  purpose:
//      build_tree builds the complete NODE tree
//
//  parameters:
//      d               in      the depth in the NODE tree
//      brd             in      the board that is analysed
//      scard           out     the SCARD sequences that are build
//
//  Copyright 2023, Gerrit A. van der Velde, Leiden

void build_tree( int d, COLOR brd[14][5][4], struct SCARD *scard[14][4] )
{
    int           nh, nl, p, s, t = st[d].t;
    struct BACKUP zbackup;

    //  loop over the nodes
    lnode[d] = lnode[d-1]->d;
    while ( lnode[d] ) {

        //  play the cards and prepare for the next ply
        st[d].n++;
        p = lnode[d]->p;
        s = lnode[d]->s;
        lnode[d]->nd = lnode[d-1]->nd + lnode[d-1]->nc;
        brd[t][s][p] ^= low_c( brd[t][s][p] & lnode[d]->c );
        if ( st[d].la && d < 4*ntricks ) {

            //  check the alpha-beta pruning
            nh = lnode[d]->nd + lnode[d]->nc + lnode[d]->nh;
            nl = lnode[d]->nd + lnode[d]->nc + lnode[d]->nl;
            if ( target.set && NS(target.p) && ( nh <= target.ns || target.ns <  nl ) ||
                 target.set && EW(target.p) && ( nh <  target.ns || target.ns <= nl ) )
                backup[t] = &zbackup;

            //  check the BACKUP hash table
            else if ( hashbackup( d+1, brd, scard ) ) {

                //  check the alpha-beta pruning
                nh = lnode[d]->nd + lnode[d]->nc + lnode[d]->nh;
                nl = lnode[d]->nd + lnode[d]->nc + lnode[d]->nl;
                if ( target.set && NS(target.p) && ( nh <= target.ns || target.ns <  nl ) ||
                     target.set && EW(target.p) && ( nh <  target.ns || target.ns <= nl ) )
                    backup[t] = &zbackup;

                //  the NODE tree contains still branches
                else if ( lnode[d]->d )
                    build_tree( d+1, brd, scard );
            }

            //  no backup NODE tree exists
            else
                build_tree( d+1, brd, scard );
        }

        //  the NODE tree is fully analysed
        else if ( st[d].la || lnode[d]->t ) ;

        //  the NODE tree contains still branches
        else if ( lnode[d]->d )
            build_tree( d+1, brd, scard );

        //  build the next ply of the NODE tree
        else {
            build_ply( d+1, (p+1)%4, brd, scard );
            build_tree( d+1, brd, scard );
        }

        //  backup the NODE tree
        bottomup_tree( d, brd, scard );
    }
}
