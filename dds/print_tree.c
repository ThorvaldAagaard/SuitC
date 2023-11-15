//  name:
//      print_tree
//
//  purpose:
//      print_tree prints a NODE tree
//
//  parameters:
//      d_start         in      the depth in the NODE tree
//      root            in      the root of the NODE tree
//      ident           in      an identification string
//
//  Copyright 2023, Gerrit A. van der Velde, Leiden

void print_tree( int d_start, struct NODE *root, char *ident )
{
    int           bo = 8, d = d_start, dc, i, k, l = 0, n = 6;
    COLOR         cards;
    char          out[446];
    static char   *blank[] = { "", "       ", "              ", "                     " };
    static char   symbols[] = "-123456789tjqka=";
    static char   seats[] = "wnes";
    static char   suits[] = "shdc--";
    struct NODE   *nnode[53];

    if ( root == NIL || d_start < 0 ) {
        printf( "\nd=%02d   id=%s\n", d_start, ident );
        return;
    }

    if ( d_start == 0 ) {
        print_tree( d_start+1, wnode[d_start]->d, ident );
        return;
    }

    //  initialise
    nnode[d-1] = wnode[d-1];
    nnode[d] = root;
    printf( "\nd=%02d   id=%-20s   t   w rnk  nd  nc  nl  nh\n", d_start, ident );

    //  fill in of unreported plies
    for ( k = 4*((d_start-1)/4)+1; k < d_start; k++ ) {
        if ( ( k & 3 ) == 1 ) {
            out[l++] = ' ';
            out[l++] = ' ';
        }
        for ( i = 0; i < n+1; i++ )
            out[l++] = ' ';
    }

    //  loop over the nodes
    while ( nnode[d] ) {

        //  handle the leader to the trick
        cards = nnode[d]->c;
        if ( ( d & 3 ) == 1 ) {
            if ( d != d_start ) {
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
        //  translate the suit into card symbols
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
        printf( "%s%s%4d%4d%4d%4d%4d%4d%4d", out, blank[4*((d+3)/4)-d], nnode[d]->t, nnode[d]->w_p, nnode[d]->rnk,
                nnode[d]->nd, nnode[d]->nc, nnode[d]->nl + dc, nnode[d]->nh + dc );
        if ( st[d].la && nnode[d]->d && d < MIN( st[MIN(d_start+bo,4*ntricks)].l+3, 4*ntricks ) )
            printf( "        t   w rnk  nd  nc  nl  nh\n" );
        else
            printf( "\n" );
        k = n + 1 + ( d & 1 == 1 ? 2 : d & 1 == 0 ? 3 : 0 );
        for ( i = l-k; i < l; i++ )
            out[i] = ' ';

        //  handle the sons
        if ( nnode[d]->d && d < MIN( st[MIN(d_start+bo,4*ntricks)].l+3, 4*ntricks ) ) {
            nnode[d+1] = nnode[d]->d;
            d++;
        }

        else {

            //  print the result on the board
            for ( k = d; k < 4*((d+3)/4); k++ )
                for ( i = 0; i < n+1; i++ )
                    out[l++] = ' ';

            //  get the next sibling
            while ( d > d_start && ! nnode[d]->r )
                d--;

            if ( d == d_start && ! nnode[d] )
                return;

            nnode[d] = nnode[d]->r;

            //  fill in of unreported plies
            if ( nnode[d] ) {
                l = 0;
                for ( k = 4*((d_start-1)/4)+1; k < d; k++ ) {
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
