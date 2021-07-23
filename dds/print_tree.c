//  name:
//      print_tree
//
//  purpose:
//      print_tree prints a NODE tree
//
//  parameters:
//      b               in      the depth in the tree
//      root            in      the root of the NODE tree
//      ident           in      an identification string

#include "header.h"

void print_tree( int b, struct NODE *root, char *ident )
{
    int           bo = 8, d = b, dc, i, k, l = 0, n = 6;
    COLOR         cards;
    char          out[446];
    static char   *blank[] = { "", "       ", "              ", "                     " };
    static char   symbols[] = "-123456789tjqka=";
    static char   seats[] = "wnes";
    static char   suits[] = "shdc-";
    struct NODE  *nnode[53];

    if ( root == NIL || b < 0 ) {
        printf( "\nply=%02d   id=%-18s\n", b, ident );
        return;
    }

    if ( b == 0 ) {
        print_tree( b+1, lnode[b]->d, ident );
        return;
    }

    //  initialise
    nnode[d-1] = lnode[d-1];
    nnode[d] = root;
    printf( "\nply=%02d   id=%-18s   t   w  qt  nd  nc  nl  nh\n", b, ident );

    //  fill in of unreported plies
    for ( k = 4*((b-1)/4)+1; k < b; k++ ) {
        if ( ( k & 3 ) == 1 ) {
            out[l++] = ' ';
            out[l++] = ' ';
        }
        for ( i = 0; i < n+1; i++ )
            out[l++] = ' ';
    }

    //  loop over the nodes
    while ( nnode[d] ) {

        nnode[d]->nd = nnode[d-1]->nd + nnode[d-1]->nc;

        //  handle the leader to the trick
        cards = nnode[d]->c;
        if ( ( d & 3 ) == 1 ) {
            if ( d != b ) {
               out[l++] = ' ';
               out[l++] = ' ';
               out[l++] = ' ';
            }
            out[l++] = seats[nnode[d]->p];
            out[l++] = ':';
        }

        //  translate the suit
        out[l++] = suits[nnode[d]->s];
        out[l++] = ':';
        //  translate the hand into card symbols
        k = l;
        if ( d )
            for ( i = 0; i < n-1; i++ )
                out[l++] = ' ';
        out[k] = '-';
        for ( i = ace+1; i >= 0; i-- ) {
            if ( set(i) & cards ) {
                out[k++] = symbols[i];
                if ( k == ( l - 1 ) )
                    break;
            }
        }

        //  print the the results
        out[l] = '\0';
        dc = nnode[d]->nd + nnode[d]->nc;
        printf( "%s%s%4d%4d%4d%4d%4d%4d%4d", out, blank[4*((d+3)/4)-d], nnode[d]->t, nnode[d]->w, nnode[d]->qt,
                nnode[d]->nd, nnode[d]->nc, nnode[d]->nl + dc, nnode[d]->nh + dc );
        if ( st[d].la && nnode[d]->d && d < MIN( st[MIN(b+bo,4*ntricks)].l+3, 4*ntricks ) )
            printf( "    w  qt  nd  nc  nl  nh\n" );
        else
            printf( "\n" );
        k = n + 1 + ( d & 1 == 1 ? 2 : d & 1 == 0 ? 3 : 0 );
        for ( i = l-k; i < l; i++ )
            out[i] = ' ';

        //  handle the sons
        if ( nnode[d]->d && d < MIN( st[MIN(b+bo,4*ntricks)].l+3, 4*ntricks ) )
            nnode[d+1] = nnode[d++]->d;

        else {

            //  print the result on the board
            for ( k = d; k < 4*((d+3)/4); k++ )
                for ( i = 0; i < n+1; i++ )
                    out[l++] = ' ';

            //  get the next sibling
            while ( d > b && ! nnode[d]->r )
                d--;

            if ( d == b && ! nnode[d] )
                return;

            nnode[d] = nnode[d]->r;

            //  fill in of unreported plies
            if ( nnode[d] ) {
                l = 0;
                for ( k = 4*((b-1)/4)+1; k < d; k++ ) {
                    if ( ( k & 3 ) == 1 )
                        l += 2;
                    l += n + 1;
                    if ( ( k & 3 ) == 0 && k+1 != d )
                        l += 3;
                }
            }
        }
    }
}
