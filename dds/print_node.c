//  name:
//      print_node
//
//  purpose:
//      print_node prints a NODE entry
//
//  parameters:
//      d               in      the depth in the NODE tree
//      root            in      the node to be printed
//      ident           in      an identification string
//
//  Copyright 2025, Gerrit A. van der Velde, Leiden

#include "header.h"

void print_node( int d, struct NODE *root, char *ident )
{
    int           i, l;
    char          out[14];
    static char   symbols[] = "-123456789tjqka=";
    static char   *seat[] = { "west", "north", "east", "south", "unknown" };
    static char   *suit[] = { "spades", "hearts", "diamonds", "clubs", "notrump", "discard" };

    if ( root )
        printf( "\nd=%02d   node=%lx", d, (long unsigned int) root );
    else {
        printf( "\nd=%02d   node=%lx ident=%s\n", d, (long unsigned int) root, ident );
        return;
    }

    l = 0;
    for ( i = ace+1; i >= 0; i-- )
        if ( set(i) & root->c )
            out[l++] = symbols[i];
    if ( l == 0 )
        out[l++] = '-';
    out[l] = '\0';

    printf( " d=%lx", (long unsigned int) root->d );
    printf( " r=%lx", (long unsigned int) root->r );
    printf( " t=%d", root->t );
    printf( "   s=%-8s", suit[root->s] );
    printf( " c=%-7s", out );
    printf( " p=%-5s", seat[root->p] );
    printf( " w=%-5s", seat[root->w_p] );
    printf( "   rnk=%x", root->rnk );
    printf( " nd=%x", root->nd );
    printf( " nc=%x", root->nc );
    printf( " nl=%x", root->nl + root->nd + root->nc );
    printf( " nh=%x", root->nh + root->nd + root->nc );
    if ( target.set )
        printf( "   t.d=%02d t.p=%-5s t.ns=%x", target.d, seat[target.p], target.ns );
    printf( "   ident=%s\n", ident );
}
