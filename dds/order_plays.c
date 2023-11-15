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
    wnode[d] = wnode[d-1]->d;
    anode->r = wnode[d];

next_node:
    while ( wnode[d]->r ) {

        bnode = wnode[d];
        nnode = bnode->r;
        while ( nnode ) {

            if ( ( wnode[d]->rnk  < nnode->rnk ) ||
                 ( wnode[d]->rnk == nnode->rnk && wnode[d]->s == nnode->s && wnode[d]->c  > nnode->c ) ||
                 ( wnode[d]->rnk == nnode->rnk && wnode[d]->s == trump    && nnode->s    != trump    ) ) {
                bnode->r = nnode->r;
                anode->r = nnode;
                nnode->r = wnode[d];
                wnode[d] = nnode;
                goto next_node;
            }

            bnode = nnode;
            nnode = bnode->r;
        }

        anode = wnode[d];
        wnode[d] = anode->r;
    }

    wnode[d] = znode.r;
    wnode[d-1]->d = znode.r;
}
