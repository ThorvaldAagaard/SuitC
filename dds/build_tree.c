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
//  Copyright 2025, Gerrit A. van der Velde, Leiden

#include "header.h"

void build_tree( int d, int b )
{
    int           nh, nl, p, s, t = st[d].t;
    struct BACKUP zbackup;

    //  loop over the nodes
    wnode[d] = wnode[d-1]->d;
    while ( wnode[d] ) {

        //  play the cards
        st[d].n++;
        p = wnode[d]->p;
        s = wnode[d]->s;
        wnode[d]->nd = wnode[d-1]->nd + wnode[d-1]->nc;
        brd[t][b][s][p] ^= low_c( scard[t][b][s]->brd[p] & wnode[d]->c );

        //  prepare for the next trick
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
            else {
                build_ply( d+1, b, wnode[d]->w_p );
                build_tree( d+1, b );
            }
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
        bottomup_tree( d, b );
    }
}
