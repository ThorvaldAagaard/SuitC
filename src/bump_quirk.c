//  name:
//      bump_quirk
//
//  purpose:
//      bump_quirk allocates space for QUIRK entries
//
//  parameters:
//      none
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

struct QUIRK *bump_quirk( void )
{
    int           i;
    struct QUIRK  *lquirk;

    if ( nextquirk->r == NIL ) {

        memory += 8192 * sizeof( struct QUIRK );
        quirk_memory += 8192 * sizeof( struct QUIRK );
        nextquirk->r = malloc( 8192 * sizeof( struct QUIRK ) );
        if ( quirk_memory > memorylimit || nextquirk->r == NIL ) {
            printf( "\nERROR: No more memory available: %6.1f MB after %3.1f seconds\n" , (float) memory / 1048576, TICK * clock() );
            exit( 1 );
        }

        lquirk = nextquirk->r;
        for ( i = 1; i < 8192; i++ ) {
            lquirk->r = lquirk + 1;
            lquirk = lquirk->r;
        }
        lquirk->r = NIL;
    }

    lquirk = nextquirk;
    nextquirk = nextquirk->r;
    lquirk->d = NIL;
    lquirk->r = NIL;
    lquirk->sc = NIL;
    lquirk->i = -1;
    lquirk->new = YES;
    lquirk->ns = tricklimit;

    qcount++;
    qcountmax = MAX( qcountmax, qcount );

    return( lquirk );
}
