//  name:
//      check_quirk
//
//  purpose:
//      check_quirk compares two quirk trees and deletes inferior ones
//
//  parameters:
//      lroot           in      the first source quirk
//      parent          in      the parent of the second source quirk
//      broot           in      the second source quirk
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

struct QUIRK *check_quirk_( struct QUIRK *lroot, struct QUIRK *parent, struct QUIRK *broot )
{
    struct QUIRK *aquirk, *bquirk, *lquirk, *nquirk;

    //  investigate the sons first
    if ( lroot->d ) {
        lquirk = lroot->d;
        while ( lquirk ) {
            bquirk = broot->d;
            while ( bquirk ) {
                if ( lquirk->ns >= bquirk->ns )
                    bquirk = check_quirk_( lquirk, broot, bquirk );
                else
                    bquirk = bquirk->r;
            }
            lquirk = lquirk->r;
        }
        if ( broot->d )
            return( broot->r );
    }

    //  relink the quirks
    nquirk = broot->r;
    if ( parent->d == broot )
        parent->d = broot->r;
    else {
        aquirk = parent->d;
        while ( aquirk->r != broot )
            aquirk = aquirk->r;
        aquirk->r = broot->r;
    }

    //  put the quirk in the free list
    broot->r = nextquirk;
    nextquirk = broot;
    qcount--;

    return( nquirk );
}

void check_quirk( struct QUIRK *rootq )
{
    struct QUIRK *bquirk, *lquirk;

    lquirk = rootq->d;
    while ( lquirk ) {
        bquirk = lquirk->r;
        while ( bquirk ) {
            if ( lquirk->new || bquirk->new )
                bquirk = check_quirk_( lquirk, rootq, bquirk);
            else
                bquirk = bquirk->r;
        }
        lquirk = lquirk->r;
    }

    //  propagate the new flag to the parent
    lquirk = rootq->d;
    while ( lquirk ) {
        rootq->new |= lquirk->new;
        lquirk->new = NO;
        lquirk = lquirk->r;
    }
}
