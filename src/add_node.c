//  name:
//      add_node
//
//  purpose:
//      add_node adds a new node to the tree
//
//  parameters:
//      d               in      the depth in the tree
//      p               in      the player who plays in this node
//      c               in      the sequence of cards played in this node
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

struct NODE *add_node( int d, int p, COLOR c )
{
    int          i, q, t = st[d].t;
    struct NODE  *tnode, *lnode;
    struct INDEX *lindex, *tindex;

    //  initialise
    tnode = nextnode;
    tnode->i = nextindex;
    lindex = wnode[d-1]->i;
    tindex = NIL;
    nruff[t][p] = nruff[t-1][p];

    //  handle ruffs
    if ( c == addcard(ruff) ) {
        if ( nruff[t][p] && ( ncardsmin[p] < t ) ) {
            nruff[t][p]--;
            while ( lindex ) {
                if ( ! lindex->c[p] ) {
                    tindex = nextindex;
                    bump_index();
                    tindex->i = lindex->i;
                    tindex->r = nextindex;
                    for ( q = 0; q < 4; q++ )
                        tindex->c[q] = lindex->c[q];
                }
                lindex = lindex->r;
            }
        }
        if ( ! tindex )
            return( NIL );
    }

    //  handle regular cards
    else if ( c ) {
        while ( lindex ) {
            if ( lindex->c[p] & c ) {
                tindex = nextindex;
                bump_index();
                tindex->i = lindex->i;
                tindex->r = nextindex;
                for ( q = 0; q < 4; q++ )
                    tindex->c[q] = lindex->c[q];
                tindex->c[p] ^= low_c( lindex->c[p] & c );
            }
            lindex = lindex->r;
        }
        if ( ! tindex )
            return( NIL );
    }

    //  handle renonces
    else {
        if ( ncardsmin[p] < t ) {
            while ( lindex ) {
                if ( ! lindex->c[p] ) {
                    tindex = nextindex;
                    bump_index();
                    tindex->i = lindex->i;
                    tindex->r = nextindex;
                    for ( q = 0; q < 4; q++ )
                        tindex->c[q] = lindex->c[q];
                }
                lindex = lindex->r;
            }
        }
        if ( ! tindex )
            return( NIL );
    }

    //  create a new node
    tindex->r = NIL;
    bump_node();
    tnode->r = wnode[d-1]->d;
    tnode->d = NIL;
    if ( c )
        tnode->c = c;
    else
        tnode->c = addcard(renonce);
    tnode->p = p;
    tnode->winc = RENONCE;

    //  insert the node in the tree
    if ( wnode[d-1]->d ) {
        tnode->l = wnode[d];
        tnode->r = NIL;
        wnode[d]->r = tnode;
        wnode[d] = tnode;
    }
    else {
        tnode->l = NIL;
        tnode->r = NIL;
        wnode[d]->r = tnode;
        wnode[d-1]->d = tnode;
        wnode[d] = tnode;
    }

    return( tnode );
}
