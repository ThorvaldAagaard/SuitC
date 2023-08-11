//  name:
//      bottomup_tree
//
//  purpose:
//      bottomup_tree determines the results and applies alpha-beta pruning
//      to get the next sibling
//
//  parameters:
//      d               in      the depth in the NODE tree
//
//  Copyright 2023, Gerrit A. van der Velde, Leiden

void bottomup_tree( int d )
{
    int           nh, nl, p, s, t = st[d].t;
    struct NODE   *nnode;

    p = lnode[d]->p;
    s = lnode[d]->s;

    nh = lnode[d]->nd + lnode[d]->nc + lnode[d]->nh;
    nl = lnode[d]->nd + lnode[d]->nc + lnode[d]->nl;

    if ( NS(p) ) lnode[d-1]->nl = MAX( lnode[d-1]->nl, lnode[d]->nc + lnode[d]->nl );
    else         lnode[d-1]->nh = MIN( lnode[d-1]->nh, lnode[d]->nc + lnode[d]->nh );

    //  save the BACKUP status
    brd[t][s][p] = scard[t][s]->brd[p];
    if ( st[d].la && d < 4*ntricks ) {
        backup[t]->node = lnode[d]->d;
        backup[t]->nh = lnode[d]->nh;
        backup[t]->nl = lnode[d]->nl;
        backup[t]->t = lnode[d]->t;
        lnode[d]->d = NIL;
    }

    //  delete all siblings if one side wins all remaining tricks
    if ( d > d_start && ( NS(p) && lnode[d-1]->nl == ntricks - t + 1 || EW(p) && lnode[d-1]->nh == 0 ) ) {

        if ( target.set && target.d == d )
            target.set = NO;

        if ( NS(p) ) lnode[d-1]->nh = lnode[d-1]->nl;
        else         lnode[d-1]->nl = lnode[d-1]->nh;

        lnode[d-1]->t = YES;
        while ( lnode[d-1]->d )
            delete_tree( d, lnode[d-1]->d );
        lnode[d] = NIL;
    }

    //  check the alpha-beta pruning root
    else if ( target.set && target.d == d ) {

        //  alpha-beta root: NS beat the NS target
        //  alpha-beta root: EW beat the EW target
        if ( ( NS(target.p) && nl > target.ns ) ||
             ( EW(target.p) && nh < target.ns ) ) {

            target.set = NO;

            while ( lnode[d-1]->d != lnode[d] )
                delete_tree( d, lnode[d-1]->d );
        }

        //  alpha-beta root: NS don't beat the NS target
        //  alpha-beta root: EW don't beat the EW target
        else if ( ( NS(target.p) && nl <= target.ns ) ||
                  ( EW(target.p) && nh >= target.ns ) ) {

            lnode[d] = delete_tree( d, lnode[d] );

            if ( lnode[d] == NIL ) {

                target.set = NO;

                if ( NS(p) ) lnode[d-1]->nh = lnode[d-1]->nl;
                else         lnode[d-1]->nl = lnode[d-1]->nh;

                lnode[d-1]->t = YES;
                while ( lnode[d-1]->d )
                    delete_tree( d, lnode[d-1]->d );
            }
        }
    }

    //  check the alpha-beta pruning status
    else if ( target.set ) {

        //  alpha-beta check: NS beat the target
        //  alpha-beta check: EW beat the target
        if ( ( NS(target.p) && NS(p) && nl >  target.ns ) ||
             ( EW(target.p) && NS(p) && nl >= target.ns ) ||
             ( NS(target.p) && EW(p) && nh <= target.ns ) ||
             ( EW(target.p) && EW(p) && nh <  target.ns ) ) {

            while ( lnode[d-1]->d != lnode[d] )
                delete_tree( d, lnode[d-1]->d );

            lnode[d-1]->t = YES;

            nnode = lnode[d-1]->d;
            while ( nnode ) {
                lnode[d-1]->t &= nnode->t;
                nnode = nnode->r;
            }

            if ( lnode[d-1]->t ) {

                if ( NS(p) ) lnode[d-1]->nh = lnode[d-1]->nl;
                else         lnode[d-1]->nl = lnode[d-1]->nh;

                while ( lnode[d-1]->d )
                    nnode = delete_tree( d, lnode[d-1]->d );
            }

            lnode[d] = NIL;
        }

        //  alpha-beta check: NS don't beat the target
        //  alpha-beta check: EW don't beat the target
        else if ( ( NS(target.p) && NS(p) && nl <= target.ns ) ||
                  ( EW(target.p) && NS(p) && nl <  target.ns ) ||
                  ( NS(target.p) && EW(p) && nh >  target.ns ) ||
                  ( EW(target.p) && EW(p) && nh >= target.ns ) ) {

            lnode[d] = lnode[d]->r;

            if ( lnode[d] == NIL ) {

                lnode[d-1]->t = YES;
                if ( NS(p) ) lnode[d-1]->nh = lnode[d-1]->nl;
                else         lnode[d-1]->nl = lnode[d-1]->nh;

                nnode = lnode[d-1]->d;
                while ( nnode ) {
                    lnode[d-1]->t &= nnode->t;
                    if ( NS(p) ) lnode[d-1]->nh = MAX( lnode[d-1]->nh, nnode->nc + nnode->nh );
                    else         lnode[d-1]->nl = MIN( lnode[d-1]->nl, nnode->nc + nnode->nl );
                    nnode = nnode->r;
                }

                if ( lnode[d-1]->t )
                    while ( lnode[d-1]->d )
                        delete_tree( d, lnode[d-1]->d );
            }
        }
    }

    //  alpha-beta pruning is inactive
    else {

        if ( d == d_start ) {
            result[nresult].s = s;
            result[nresult].c = lnode[d]->c;
            result[nresult].ns = nh;
            nresult++;
        }

        lnode[d] = delete_tree( d, lnode[d] );

        if ( lnode[d] && d >= d_ab ) {
            target.d = d;
            target.p = p;
            target.ns = NS(p) ? nl : nh;
            target.set = YES;
        }

        if ( lnode[d] == NIL ) {
            lnode[d-1]->t = YES;
            if ( NS(p) ) lnode[d-1]->nh = lnode[d-1]->nl;
            else         lnode[d-1]->nl = lnode[d-1]->nh;
        }
    }
}
