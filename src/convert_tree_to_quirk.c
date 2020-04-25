//  name:
//      convert_tree_to_quirk
//
//  purpose:
//      convert_tree_to_quirk converts the tree to quirks. For each
//      terminal node it builds a QUIRK with a call to bump_quirk.
//      Thereafter it combines NS nodes with calls to fold_quirk,
//      and EW nodes by calls to merge_quirk.
//
//  parameters:
//      d               in      the depth in the tree
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void convert_tree_to_quirk( int d )
{
    int           i, m, term, z;
    struct INDEX  *lindex;
    struct QUIRK  *lquirk;
    struct QUIRKD *wquirkd;
    struct QUIRKH *wquirkh;
    struct TRICKS *trck;

    //  the tree contains no more branches
    if ( wnode[d]->term ) {

        if ( qhd == 1 ) {
            tquirk[d] = bump_quirk();
            lquirk = tquirk[d];
            lindex = wnode[d]->i;
            while ( lindex ) {
                lquirk->d = bump_quirk();
                lquirk = lquirk->d;
                lquirk->i = lindex->i;
                lquirk->ns = lindex->max;
                lindex = lindex->r;
            }
        }

        if ( qhd == 2 ) {
            i = -1;
            tquirkd[d].h = nextquirkh;
            #include "bump_quirkh.c"
            tquirkd[d].h->d = nextquirkd;
            #include "bump_quirkd.c"
            lindex = wnode[d]->i;
            while ( lindex ) {
                tquirkd[d].h->t[++i].i = lindex->i;
                tquirkd[d].h->t[i].ns = lindex->max;
                lindex = lindex->r;
            }
            tquirkd[d].h->d->il = 0;
            tquirkd[d].h->d->ih = i;
        }
    }

    //  build the quirk tree for the sons
    else {

        tquirk[d] = NIL;
        tquirkd[d].h = NIL;
        for ( term = 1; term >= 0; term-- ) {

            wnode[d+1] = wnode[d]->d;
            while ( wnode[d+1] ) {

                //  build the quirk tree for the sons
                if ( wnode[d+1]->term == term ) {

                    convert_tree_to_quirk( d+1 );

                    if ( qhd == 1 ) {

                        if ( tquirk[d] == NIL )
                            tquirk[d] = tquirk[d+1];

                        //  fold north-south QUIRK trees
                        else if ( NS( wnode[d+1]->p ) ) {
                            fold_quirk( tquirk[d+1]->d, tquirk[d] );
                            delete_quirk( tquirk[d+1] );
                        }

                        //  merge east-west QUIRK trees
                        else if ( EW( wnode[d+1]->p ) ) {

                            //  reduce the number of quirks
                            if ( cutoff && ( ( (double) count_quirk( tquirk[d] ) * (double) count_quirk( tquirk[d+1] ) ) > cutoff ) )
                                limit_quirk( tquirk[d], tquirk[d+1] );

                            lquirk = bump_quirk();
                            if ( tquirk[d+1]->d->i <= tquirk[d]->d->i )
                                merge_quirk( tquirk[d+1]->d, tquirk[d]->d, lquirk );
                            else
                                merge_quirk( tquirk[d]->d, tquirk[d+1]->d, lquirk );

                            delete_quirk( tquirk[d] );
                            delete_quirk( tquirk[d+1] );
                            tquirk[d] = lquirk;
                        }
                    }

                    if ( qhd == 2 ) {

                        if ( tquirkd[d].h == NIL )
                            tquirkd[d].h = tquirkd[d+1].h;

                        //  fold north-south QUIRKH trees
                        else if ( NS( wnode[d+1]->p ) )
                            fold_quirkh( d );

                        //  merge east-west QUIRKH trees
                        else if ( EW( wnode[d+1]->p ) ) {

                            //  reduce the number of quirks
                            if ( cutoff && d <= 4 && ( ( (double) count_quirkh( &tquirkd[d] ) * (double) count_quirkh( &tquirkd[d+1] ) ) > cutoff ) )
                                limit_quirkh( d );

                            merge_quirkh( d );
                        }
                    }
                }

                wnode[d+1] = wnode[d+1]->r;
            }
        }
    }
}
