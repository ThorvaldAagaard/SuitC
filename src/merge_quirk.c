//  name:
//      merge_quirk
//
//  purpose:
//      merge_quirk merges two quirk trees
//
//  parameters:
//      quirk1          in      the first source quirk
//      quirk2          in      the second source quirk
//      tquirk          in      the destination quirk
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void merge_quirk( struct QUIRK *quirk1, struct QUIRK *quirk2, struct QUIRK *tquirk )
{
    int          d = 0, ns;
    struct QUIRK *bquirk[NBOARDS], *lquirk[NBOARDS], *nquirk;

    lquirk[d] = quirk1;
    bquirk[d] = quirk2;
    if ( lquirk[d] && bquirk[d] ) {

        if ( lquirk[d]->i < bquirk[d]->i ) {
            while ( lquirk[d] ) {
                nquirk = add_quirk( lquirk[d]->i, lquirk[d]->ns, tquirk );
                merge_quirk( lquirk[d]->d, bquirk[d], nquirk );
                lquirk[d] = lquirk[d]->r;
            }
        }

        else if ( lquirk[d]->i == bquirk[d]->i ) {
            while ( lquirk[d] ) {
                bquirk[d] = quirk2;
                while ( bquirk[d] ) {
                    ns = lquirk[d]->ns < bquirk[d]->ns ? lquirk[d]->ns : bquirk[d]->ns;
                    nquirk = add_quirk( lquirk[d]->i, ns, tquirk );
                    if ( lquirk[d]->d || bquirk[d]->d )
                        merge_quirk( lquirk[d]->d, bquirk[d]->d, nquirk );
                    bquirk[d] = bquirk[d]->r;
                }
                lquirk[d] = lquirk[d]->r;
            }
        }

        else if ( lquirk[d]->i > bquirk[d]->i ) {
            while ( bquirk[d] ) {
                nquirk = add_quirk( bquirk[d]->i, bquirk[d]->ns, tquirk );
                merge_quirk( lquirk[d], bquirk[d]->d, nquirk );
                bquirk[d] = bquirk[d]->r;
            }
        }
    }

    else if ( lquirk[d] ) {
        while ( lquirk[d] ) {
            nquirk = add_quirk( lquirk[d]->i, lquirk[d]->ns, tquirk );
            if ( lquirk[d]->d )
                merge_quirk( lquirk[d]->d, bquirk[d], nquirk );
            lquirk[d] = lquirk[d]->r;
        }
    }

    else if ( bquirk[d] ) {
        while ( bquirk[d] ) {
            nquirk = add_quirk( bquirk[d]->i, bquirk[d]->ns, tquirk );
            if ( bquirk[d]->d )
                merge_quirk( lquirk[d], bquirk[d]->d, nquirk );
            bquirk[d] = bquirk[d]->r;
        }
    }

    check_quirk( tquirk );
}
