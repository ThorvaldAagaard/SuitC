//  name:
//      build_ply
//
//  purpose:
//      build_ply builds the NODE tree for a ply
//
//  parameters:
//      d               in      the depth in the NODE tree
//      p               in      the player of this ply

//  #include "header.h"

void build_ply( int d, int p )
{
    int           b, i, lds, n, q, s, t = st[d].t;
    COLOR         c[14], cards;
    struct NODE   znode;

    //  initialise
    lnode[d] = &znode;
    znode.r = NIL;

    for ( s = 0; s < 4; s++ ) {

        lds = st[d].ld ? s : lnode[st[d].l]->s;
        if ( brd[t][s][p] && ( st[d].ld || s == lds || ! brd[t][lds][p] ) ) {

            cards = brd[t][s][(p+1)%4] | brd[t][s][(p+2)%4] | brd[t][s][(p+3)%4];
            for ( b = st[d].l; b < d; b++ )
                if ( lnode[b]->s == s )
                    cards |= st[b].brd;

            n = -1;
            q = unknown;
            for ( i = ace; i >= two; i-- ) {
                if ( brd[t][s][p] & set(i) ) {
                    if ( p != q ) {
                        n++;
                        q = p;
                        c[n] = set(i);
                    }
                    else
                        c[n] |= set(i);
                }
                else if ( cards & set(i) )
                    q = unknown;
            }

            for ( i = 0; i <= n; i++ )
                build_node( d, s, p, c[i] );
        }
    }

    //  order the plays
    lnode[d] = znode.r;
    lnode[d-1]->d = znode.r;
    order_plays( d );
}
