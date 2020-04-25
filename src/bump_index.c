//  name:
//      bump_index
//
//  purpose:
//      bump_index allocates space for index entries
//
//  parameters:
//      none
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

struct INDEX *bump_index( void )
{
    int           i;
    struct INDEX  *lindex;

    if ( nextindex->r == NIL ) {

        memory += 16384 * sizeof( struct INDEX );
        nextindex->r = malloc( 16384 * sizeof( struct INDEX ) );
        if ( nextindex->r == NIL ) {
            printf( "\nERROR: No more memory available: %6.1f MB after %3.1f seconds\n" , (float) memory / 1048576, TICK * clock() );
            exit( 1 );
        }

        lindex = nextindex->r;
        for ( i = 1; i < 16384; i++ ) {
            lindex->r = lindex + 1;
            lindex = lindex->r;
        }
        lindex->r = NIL;
    }

    lindex = nextindex;
    nextindex = nextindex->r;
    icount++;
    icountmax = MAX( icountmax, icount );

    return( lindex );
}
