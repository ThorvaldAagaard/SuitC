//  name:
//      fold_quirk
//
//  purpose:
//      fold_quirk folds two quirk trees
//
//  parameters:
//      quirk1          in      the first quirk
//      quirk2          in      the second quirk
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void fold_quirk( struct QUIRK *quirk1, struct QUIRK *quirk2 )
{
    int          d = 1;
    struct QUIRK *lquirk[NBOARDS+1], *tquirk[NBOARDS+1];

    //  initialise
    tquirk[0] = quirk2;
    lquirk[1] = quirk1;

    //  loop over the tree
    while ( lquirk[d] ) {

        tquirk[d] = add_quirk( lquirk[d]->i, lquirk[d]->ns, tquirk[d-1] );

        //  continue with the sons
        if ( lquirk[d]->d ) {
            d++;
            lquirk[d] = lquirk[d-1]->d;
        }

        //  get the next quirk
        else {
            while ( ( d > 1 ) && ! lquirk[d]->r ) {
                d--;
                check_quirk( tquirk[d] );
            }
            lquirk[d] = lquirk[d]->r;
        }
    }

    //  check the new quirk
    check_quirk( tquirk[0] );
}
