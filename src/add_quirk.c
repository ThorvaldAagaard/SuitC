//  name:
//      add_quirk
//
//  purpose:
//      add_quirk adds a new quirk to the quirk tree
//
//  parameters:
//      b               in      the board number
//      ns              in      the number of NS tricks
//      rootq           in      the parent quirk
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

struct QUIRK *add_quirk( int b, int ns, struct QUIRK *rootq )
{
    struct QUIRK *aquirk, *lquirk;

    aquirk = NIL;
    if ( rootq->d ) {
        lquirk = rootq->d;
        while ( lquirk && lquirk->ns > ns ) {
            aquirk = lquirk;
            lquirk = lquirk->r;
        }

        //  return existing quirk
        if ( lquirk && lquirk->ns == ns )
            return( lquirk );
    }

    //  add a new quirk
    lquirk = bump_quirk();
    if ( aquirk ) {
        lquirk->r = aquirk->r;
        aquirk->r = lquirk;
    }
    else {
        lquirk->r = rootq->d;
        rootq->d = lquirk;
    }
    lquirk->i = b;
    lquirk->ns = ns;

    return( lquirk );
}
