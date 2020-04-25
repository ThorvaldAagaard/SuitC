//  name:
//      delete_quirk
//
//  purpose:
//      delete_quirk deletes a QUIRK tree
//
//  parameters:
//      rootq           in      the root off the QUIRK tree that is deleted
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void delete_quirk( struct QUIRK *rootq )
{
    int          d = 0;
    struct QUIRK *lquirk[NBOARDS+1], *nquirk;

    //  initialise
    lquirk[d] = rootq;

    //  loop over the tree
    while ( lquirk[d] ) {

        //  investigate the sons first
        if ( lquirk[d]->d ) {
            d++;
            lquirk[d] = lquirk[d-1]->d;
        }

        //  delete the quirk
        else {
            nquirk = lquirk[d]->r;
            lquirk[d]->r = nextquirk;
            nextquirk = lquirk[d];
            qcount--;
            lquirk[d] = nquirk;
        }

        //  delete the quirks
        while ( d && ! lquirk[d] ) {
            d--;
            nquirk = lquirk[d]->r;
            lquirk[d]->r = nextquirk;
            nextquirk = lquirk[d];
            qcount--;
            lquirk[d] = nquirk;
        }
    }
}
