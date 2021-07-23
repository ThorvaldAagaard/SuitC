//  name:
//      delete_tree
//
//  purpose:
//      delete_tree deletes a NODE tree
//
//  parameters:
//      b               in      the depth in the tree
//      root            in      the root of the NODE tree to be deleted

//  #include "header.h"

struct NODE *delete_tree( int b, struct NODE *root )
{
    int           d = b;
    struct NODE   *anode, *bnode[53], *nnode;

    //  unlink root from the tree
    nnode = root->r;
    if ( lnode[b-1]->d == root ) {
        lnode[b-1]->d = root->r;
    }
    else {
        anode = lnode[b-1]->d;
        while ( anode->r != root )
            anode = anode->r;
        anode->r = root->r;
    }
    root->r = NIL;

    //  delete the tree
    bnode[d] = root;
    while ( bnode[d] ) {

        if ( bnode[d]->d && ! st[d].la )
            bnode[d+1] = bnode[d]->d;
        else
            bnode[d+1] = NIL;
        d++;

        while ( d > b && ! bnode[d] ) {
            d--;
            ncount--;
            anode = bnode[d]->r;
            bnode[d]->r = nextnode;
            nextnode = bnode[d];
            bnode[d] = anode;
        }
    }

    return( nnode );
}
