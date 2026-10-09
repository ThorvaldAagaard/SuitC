//  name:
//      print_seqc_tree
//
//  purpose:
//      print_seqc_tree prints a SEQC tree
//
//  parameters:
//      seqc            in      the root of the SEQC tree
//
//  Copyright 2026, Gerrit A. van der Velde, Leiden

#include "header.h"

void print_seqc_tree( struct SEQC *seqc )
{
    int         d = 1, i;
    static char *seats[] = { "W", "N", "E", "S", "-" };
    struct SEQC *lseqc[NPLIES+1];

    //  handle NIL entries
    if ( ! seqc ) {
        printf( "   %s:%x:%04x\n", "-", 0, 0 );
        return;
    }

    //  loop over the SEQC tree
    lseqc[d] = seqc;
    while ( lseqc[d] ) {

        printf( "   %s:%x:%04x", seats[lseqc[d]->p], lseqc[d]->n, lseqc[d]->c >> 2 );

        if ( lseqc[d]->d ) {
            lseqc[d+1] = lseqc[d]->d;
            d++;
        }

        else {
            printf( "\n" );
            while ( d && ! lseqc[d]->r )
                d--;
            if ( ! d )
                break;
            lseqc[d] = lseqc[d]->r;
            for ( i = 1; i < d; i++ )
                printf( "           " );
        }
    }
}
