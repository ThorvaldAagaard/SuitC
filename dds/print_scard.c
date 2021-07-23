//  name:
//      print_scard
//
//  purpose:
//      print_scard prints a SCARD tree
//
//  parameters:
//      root            in      the SCARD tree or list to be printed
//      id              in      an identification string

#include "header.h"

void print_scard( struct SCARD *root, char *id )
{
    int           i, k, l;
    char          out[160], str[10];
    static char   seats[] = "wnes";
    static char   symbols[] = "-123456789tjqka=";
    static char   *suit[] = { "spades", "hearts", "diamonds", "clubs", "notrump", "discard" };
    struct SCARD  *lc;

    if ( ! root )
        return;

    l = 0;
    lc = root;
    while ( lc ) {

        k = 0;
        for ( i = ace+1; i >= 0; i-- ) {
            if ( set(i) & lc->c )
                str[k++] = symbols[i];
            if ( k == 5 )
                break;
        }

        str[k] = '\0';

        sprintf( &out[l], " %c:%x:%-6s", seats[lc->p], lc->n, str );
        l += 10;

        lc = lc->d;
    }

    out[l] = '\0';
    printf( "%-10s%-10s%s\n", suit[root->s], id, out );
}
