//  name:
//      print_tree
//
//  purpose:
//      print_tree prints a tree
//
//  parameters:
//      d_              in      the depth in the NODE tree
//      rootn           in      the root of the tree
//      printboards     in      flag to print the boards
//      printsiblings   in      flag to print the siblings of the root
//
//  Copyright 2020, Gerrit A. van der Velde, Leiden

//  #include "header.h"

void print_tree( int d_, struct NODE *rootn, int printboards, int printsiblings )
{
    int          b, d, i, k, l, w[NPLIES+1], wlen;
    char         out[800];
    COLOR        cards;
    static char  hexdigit[] = "0123456789abcdef";
    static char  *symbols[] = { "-123456789tjqka=", "-123456789tbvha=", "-123456789TJQKA=", "-123456789TBVHA=" };
    static char  seats[] = "wnes";
    struct INDEX *lindex;
    struct NODE  *lnode[NPLIES+1];
    struct SCARD *sc;

    //  return if the root of the tree is the NIL entry
    if ( rootn == NIL )
        return;

    //  initialise
    if ( d_ == 0 && maxply ) {
        d_++;
        rootn = rootn->d;
    }

    for ( d = d_; d <= maxply; d++ )
        w[d] = 1;

    //  determine the length of the output strings
    d = d_;
    lnode[d] = rootn;
    while ( lnode[d] ) {

        k = 0;
        for ( i = ace+1; i >= 0; i-- )
            if ( ( addcard(i) & lnode[d]->c ) && ! ( addcard(i+1) & lnode[d]->c ) )
                k++;
        w[d] = MAX( w[d], k );

        if ( lnode[d]->d ) {
            d++;
            lnode[d] = lnode[d-1]->d;
        }
        else {
            while ( d > d_ && ! lnode[d]->r )
                d--;
            lnode[d] = lnode[d]->r;
        }
    }

    wlen = 2 * (maxply-d_+1) + 2 * ( (maxply-d_+1)/4) + 2 * ( (maxply-d_)/4) + 4;
    for ( d = d_; d <= maxply; d++ )
        wlen += w[d];

    //  print the board header
    for ( l = 0; l < wlen; l++ )
        out[l] = ' ';
    if ( printboards ) {
        for ( i = ( DUALHEAD ? z_count-1 : ew_c_count-1 ); i >= 0; i-- ) {
            l = wlen;
            for ( k = 0; k < boardcount; k++ ) { out[l++] = ' '; out[l++] = b_print[i][k]; }
            out[l++] = '\0'; printf( "%s\n", out );
        }
    }

    //  loop over the nodes
    l = 0;
    d = d_;
    lnode[d] = rootn;
    while ( lnode[d] ) {

        //  handle the leader to the trick
        if ( ( d & 0x3 ) == 0x1 ) { out[l++] = seats[lnode[d]->p]; out[l++] = ':'; }

        //  complete the set of the played cards
        cards = lnode[d]->c;
        sc = &scards[0][lnode[d]->p];
        for ( i = 0; i <= sc->n; i++ )
            if ( cards & sc->c[i] )
                cards |= sc->c[i];

        //  translate the hand into card symbols
        k = l;
        for ( i = 0; i < w[d]+2; i++ ) out[l++] = ' ';
        for ( i = ace+1; i >= 0; i-- )
            if ( ( addcard(i) & cards ) && ! ( addcard(i+1) & cards ) )
                out[k++] = symbols[printoption][i];
        if ( ( d & 0x3 ) == 0x0 ) { out[l++] = ' '; out[l++] = ' '; }

        //  handle the sons
        if ( lnode[d]->d ) {
            d++;
            lnode[d] = lnode[d-1]->d;
        }

        else {

            //  fill in of unmade plies
            for ( b = d+1; b <= maxply; b++ ) {
                if ( ( b & 0x3 ) == 0x1 ) { out[l++] = ' '; out[l++] = ' '; }
                out[l++] = '.';
                for ( i = 0; i < w[b]+1; i++ ) out[l++] = ' ';
                if ( ( b & 0x3 ) == 0x0 ) { out[l++] = ' '; out[l++] = ' '; }
            }

            //  print the result on the board
            out[l++] = ( defence ? 'e' : 'n' );
            out[l++] = ( defence ? 'w' : 's' );
            out[l++] = '=';
            k = l;
            for ( i = 0; i < boardcount; i++ ) { out[l++] = '-'; out[l++] = ' '; }
            out[l] = '\0';
            lindex = lnode[d]->i;
            while( lindex ) {
                out[k+2*lindex->i] = ( defence ? hexdigit[tricklimit-lindex->min] : hexdigit[lindex->max] );
                lindex = lindex->r;
            }
            printf( "%s\n", out );

            //  get the next sibling
            while ( d > d_ && ! lnode[d]->r )
                d--;
            if ( d == d_ && ! ( printsiblings && lnode[d]->r ) )
                break;
            lnode[d] = lnode[d]->r;

            //  fill in the leading unreported plies
            l = 0;
            for ( b = d_; b < d; b++ ) {
                if ( ( b & 0x3 ) == 0x1 ) { out[l++] = ' '; out[l++] = ' '; }
                if ( ( b & 0x3 ) == 0x0 ) { out[l++] = ' '; out[l++] = ' '; }
                for ( k = 0; k < w[b]+2; k++ ) out[l++] = ' ';
            }
        }
    }
}
