//  name:
//      bump_node
//
//  purpose:
//      bump_node allocates space for node entries
//
//  parameters:
//      none
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

struct NODE *bump_node( void )
{
    int           i;
    struct NODE  *lnode;

    if ( nextnode->r == NIL ) {

        memory += 8192 * sizeof( struct NODE );
        nextnode->r = malloc( 8192 * sizeof( struct NODE ) );
        if ( nextnode->r == NIL ) {
            printf( "\nERROR: No more memory available: %6.1f MB after %3.1f seconds\n" , (float) memory / 1048576, TICK * clock() );
            exit( 1 );
        }

        lnode = nextnode->r;
        for ( i = 1; i < 8192; i++ ) {
            lnode->r = lnode + 1;
            lnode = lnode->r;
        }
        lnode->r = NIL;
    }

    ncount++;
    ncountmax = MAX( ncountmax, ncount );
    lnode = nextnode;
    nextnode = nextnode->r;
    lnode->d = NIL;
    lnode->l = NIL;
    lnode->r = NIL;

    return( lnode );
}
