//  name:
//      build_ply
//
//  purpose:
//      build_ply builds the NODE tree for a ply
//
//  parameters:
//      d               in      the depth in the NODE tree
//      p               in      the player of this ply
//
//  Copyright 2023, Gerrit A. van der Velde, Leiden

void build_ply( int d, int p )
{
    int           lds, n, s, t = st[d].t;
    struct NODE   znode;
    struct SEQC   *sq;

    //  initialise
    lnode[d] = &znode;
    znode.r = NIL;

    for ( s = 0; s < 4; s++ ) {
        lds = st[d].ld ? s : lnode[st[d].l]->s;
        if ( brd[t][s][p] && ( st[d].ld || s == lds || ! brd[t][lds][p] ) ) {
            sq = scard[t][s]->sq;
            for ( n = 0; n < scard[t][s]->n; n++ )
                if ( sq[n].p == p )
                    build_node( d, s, p, sq[n].c );
        }
    }

    //  order the plays
    lnode[d] = znode.r;
    lnode[d-1]->d = znode.r;
    order_plays( d );
}
