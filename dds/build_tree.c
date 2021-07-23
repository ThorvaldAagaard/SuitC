//  name:
//      build_tree
//
//  purpose:
//      build_tree builds the complete NODE tree
//
//  parameters:
//      none

//  #include "header.h"

void build_tree( void )
{
    int           d = d_start, i, lds, nh, nl, ns, p, s, t;
    struct BACKUP zbackup;

    //  loop over the nodes
    while ( lnode[d] ) {

        st[d].n++;
        t = st[d].t;

        p = lnode[d]->p;
        s = lnode[d]->s;
        lds = lnode[st[d].l]->s;

        //  handle the play of the cards
        brd[t][s][p] ^= low_c( brd[t][s][p] & lnode[d]->c );

        //  generate the next trick
        lnode[d]->nd = lnode[d-1]->nd + lnode[d-1]->nc;
        if ( st[d].la && d < 4*ntricks ) {

            //  check the alpha-beta pruning
            nh = lnode[d]->nd + lnode[d]->nc + lnode[d]->nh;
            nl = lnode[d]->nd + lnode[d]->nc + lnode[d]->nl;
            if ( ( target.set && NS(target.p) && ( nh <= target.ns || target.ns <  nl ) ) ||
                 ( target.set && EW(target.p) && ( nh <  target.ns || target.ns <= nl ) ) ) {
                backup[t] = &zbackup;
                lnode[d+1] = NIL;
            }

            //  check the BACKUP hash table
            else if ( hashbackup( d+1 ) ) {

                //  check the alpha-beta pruning
                nh = lnode[d]->nd + lnode[d]->nc + lnode[d]->nh;
                nl = lnode[d]->nd + lnode[d]->nc + lnode[d]->nl;
                if ( ( target.set && NS(target.p) && ( nh <= target.ns || target.ns <  nl ) ) ||
                     ( target.set && EW(target.p) && ( nh <  target.ns || target.ns <= nl ) ) ) {
                    backup[t] = &zbackup;
                    lnode[d+1] = NIL;
                }

                else
                    lnode[d+1] = lnode[d]->d;
            }

            else
                lnode[d+1] = lnode[d]->d;
        }

        else if ( st[d].la || lnode[d]->t || lnode[d]->d )
            lnode[d+1] = lnode[d]->d;

        else {
            build_ply( d+1, (p+1)%4 );
            lnode[d+1] = lnode[d]->d;
        }
        d++;

        //  get the next node
        while ( d && ! lnode[d] ) {

            d--;
            if ( d < d_start )
                return;

            bottomup_tree( d );
        }
    }
}
