//  name:
//      order_plays
//
//  purpose:
//      order_plays orders the NODE plays
//
//  parameters:
//      d               in      the depth in the NODE tree
//
//  Copyright 2023, Gerrit A. van der Velde, Leiden

void order_plays( int d )
{
    struct NODE   *anode, *bnode, *nnode, znode;

    anode = &znode;
    lnode[d] = lnode[d-1]->d;
    anode->r = lnode[d];

next_node:
    while ( lnode[d]->r ) {

        bnode = lnode[d];
        nnode = bnode->r;
        while ( nnode ) {

            if ( ( lnode[d]->rnk  < nnode->rnk ) ||
                 ( lnode[d]->rnk == nnode->rnk && lnode[d]->s == nnode->s && lnode[d]->c  > nnode->c ) ||
                 ( lnode[d]->rnk == nnode->rnk && lnode[d]->s == trump    && nnode->s    != trump    ) ) {
                bnode->r = nnode->r;
                anode->r = nnode;
                nnode->r = lnode[d];
                lnode[d] = nnode;
                goto next_node;
            }

            bnode = nnode;
            nnode = bnode->r;
        }

        anode = lnode[d];
        lnode[d] = anode->r;
    }

    lnode[d] = znode.r;
    lnode[d-1]->d = znode.r;
}
