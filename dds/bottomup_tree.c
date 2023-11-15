//  name:
//      bottomup_tree
//
//  purpose:
//      bottomup_tree determines the results and applies alpha-beta pruning
//      to get the next sibling
//
//  parameters:
//      d               in      the depth in the NODE tree
//      brd             in      the board that is analysed
//      scard           out     the SCARD sequences that are build
//
//  Copyright 2023, Gerrit A. van der Velde, Leiden

void bottomup_tree( int d, COLOR brd[14][5][4], struct SCARD *scard[14][4] )
{
    int           nh, nl, p, s, t = st[d].t;
    struct NODE   *nnode;

    p = wnode[d]->p;
    s = wnode[d]->s;

    nh = wnode[d]->nd + wnode[d]->nc + wnode[d]->nh;
    nl = wnode[d]->nd + wnode[d]->nc + wnode[d]->nl;

    if ( NS(p) ) wnode[d-1]->nl = MAX( wnode[d-1]->nl, wnode[d]->nc + wnode[d]->nl );
    else         wnode[d-1]->nh = MIN( wnode[d-1]->nh, wnode[d]->nc + wnode[d]->nh );

    //  save the BACKUP status
    brd[t][s][p] = scard[t][s]->brd[p];
    if ( st[d].la && d < 4*ntricks ) {
        backup[t]->node = wnode[d]->d;
        backup[t]->nh = wnode[d]->nh;
        backup[t]->nl = wnode[d]->nl;
        backup[t]->t = wnode[d]->t;
        wnode[d]->d = NIL;
    }

    //  delete all siblings if one side wins all remaining tricks
    if ( d > d_start && ( NS(p) && wnode[d-1]->nl == ntricks - t + 1 || EW(p) && wnode[d-1]->nh == 0 ) ) {

        if ( target.set && target.d == d )
            target.set = NO;

        if ( NS(p) ) wnode[d-1]->nh = wnode[d-1]->nl;
        else         wnode[d-1]->nl = wnode[d-1]->nh;

        wnode[d-1]->t = YES;
        while ( wnode[d-1]->d )
            delete_tree( d, wnode[d-1]->d );
        wnode[d] = NIL;
    }

    //  check the alpha-beta pruning root
    else if ( target.set && target.d == d ) {

        //  alpha-beta root: NS beat the NS target
        //  alpha-beta root: EW beat the EW target
        if ( ( NS(target.p) && nl > target.ns ) ||
             ( EW(target.p) && nh < target.ns ) ) {

            target.set = NO;

            while ( wnode[d-1]->d != wnode[d] )
                delete_tree( d, wnode[d-1]->d );
        }

        //  alpha-beta root: NS don't beat the NS target
        //  alpha-beta root: EW don't beat the EW target
        else if ( ( NS(target.p) && nl <= target.ns ) ||
                  ( EW(target.p) && nh >= target.ns ) ) {

            wnode[d] = delete_tree( d, wnode[d] );

            if ( wnode[d] == NIL ) {

                target.set = NO;

                if ( NS(p) ) wnode[d-1]->nh = wnode[d-1]->nl;
                else         wnode[d-1]->nl = wnode[d-1]->nh;

                wnode[d-1]->t = YES;
                while ( wnode[d-1]->d )
                    delete_tree( d, wnode[d-1]->d );
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

            while ( wnode[d-1]->d != wnode[d] )
                delete_tree( d, wnode[d-1]->d );

            wnode[d-1]->t = YES;

            nnode = wnode[d-1]->d;
            while ( nnode ) {
                wnode[d-1]->t &= nnode->t;
                nnode = nnode->r;
            }

            if ( wnode[d-1]->t ) {

                if ( NS(p) ) wnode[d-1]->nh = wnode[d-1]->nl;
                else         wnode[d-1]->nl = wnode[d-1]->nh;

                while ( wnode[d-1]->d )
                    nnode = delete_tree( d, wnode[d-1]->d );
            }

            wnode[d] = NIL;
        }

        //  alpha-beta check: NS don't beat the target
        //  alpha-beta check: EW don't beat the target
        else if ( ( NS(target.p) && NS(p) && nl <= target.ns ) ||
                  ( EW(target.p) && NS(p) && nl <  target.ns ) ||
                  ( NS(target.p) && EW(p) && nh >  target.ns ) ||
                  ( EW(target.p) && EW(p) && nh >= target.ns ) ) {

            wnode[d] = wnode[d]->r;

            if ( wnode[d] == NIL ) {

                wnode[d-1]->t = YES;
                if ( NS(p) ) wnode[d-1]->nh = wnode[d-1]->nl;
                else         wnode[d-1]->nl = wnode[d-1]->nh;

                nnode = wnode[d-1]->d;
                while ( nnode ) {
                    wnode[d-1]->t &= nnode->t;
                    if ( NS(p) ) wnode[d-1]->nh = MAX( wnode[d-1]->nh, nnode->nc + nnode->nh );
                    else         wnode[d-1]->nl = MIN( wnode[d-1]->nl, nnode->nc + nnode->nl );
                    nnode = nnode->r;
                }

                if ( wnode[d-1]->t )
                    while ( wnode[d-1]->d )
                        delete_tree( d, wnode[d-1]->d );
            }
        }
    }

    //  alpha-beta pruning is inactive
    else {

        if ( d == d_start ) {
            result[nresult].s = s;
            result[nresult].c = wnode[d]->c;
            result[nresult].ns = nh;
            nresult++;
        }

        wnode[d] = delete_tree( d, wnode[d] );

        if ( wnode[d] && d >= d_ab ) {
            target.d = d;
            target.p = p;
            target.ns = NS(p) ? nl : nh;
            target.set = YES;
        }

        if ( wnode[d] == NIL ) {
            wnode[d-1]->t = YES;
            if ( NS(p) ) wnode[d-1]->nh = wnode[d-1]->nl;
            else         wnode[d-1]->nl = wnode[d-1]->nh;
        }
    }
}
