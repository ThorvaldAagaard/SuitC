//  name:
//      build_node
//
//  purpose:
//      build_node builds a NODE entry
//
//  parameters:
//      d               in      the depth in the NODE tree
//      b               in      the board that is analysed
//      s               in      the suit of this node
//      p               in      the player of this node
//      c               in      the cards of this node
//
//  Copyright 2023, Gerrit A. van der Velde, Leiden

void build_node( int d, int b, int s, int p, COLOR c )
{
    int           l, lds, q, t = st[d].t;
    struct        { int p, s; COLOR c; } w;

    ncount++;
    ncalls[0]++;
    lnode[d]->r = nextnode;
    if ( ncount > ncountmax ) {
        ncountmax++;
        nextnode++;
        if ( ncountmax == NNODE ) {
            printf( "No more memory for the NODE tree; increase the value of NNODE\n" );
            exit( 99 );
        }
    }
    else
        nextnode = nextnode->r;
    lnode[d] = lnode[d]->r;
    lnode[d]->d = NIL;
    lnode[d]->r = NIL;

    if ( st[d].ld || s == lnode[d-1]->w_s && c > lnode[d-1]->w_c || s == trump && lnode[d-1]->w_s != trump ) {
        lnode[d]->w_p = p;
        lnode[d]->w_s = s;
        lnode[d]->w_c = c;
    }
    else {
        lnode[d]->w_p = lnode[d-1]->w_p;
        lnode[d]->w_c = lnode[d-1]->w_c;
        lnode[d]->w_s = lnode[d-1]->w_s;
    }

    lnode[d]->p = p;
    lnode[d]->s = s;
    lnode[d]->c = c;
    lnode[d]->nc = st[d].la && NS(lnode[d]->w_p) ? 1 : 0;
    lnode[d]->nd = lnode[d-1]->nc + lnode[d-1]->nd;
    lnode[d]->nh = ntricks - d/4;
    lnode[d]->nl = 0;
    lnode[d]->t = d == 4*ntricks ? YES : NO;

    w.p = lnode[d]->w_p;
    w.s = lnode[d]->w_s;
    w.c = lnode[d]->w_c;
    lds = lnode[st[d].l]->s;

    for ( l = d+1; l <= st[d].l+3; l++ ) {
        q = (p+l-d)%4;
        if ( w.s == lds && brd[b][t][lds][q] > w.c ) {
            w.p = q;
            w.s = lds;
            w.c = brd[b][t][lds][q];
        }
        if ( ! brd[b][t][lds][q] && ( w.s == trump && brd[b][t][trump][q] > w.c || w.s != trump && brd[b][t][trump][q] ) ) {
            w.p = q;
            w.s = trump;
            w.c = brd[b][t][trump][q];
        }
    }

    lnode[d]->rnk = NS(lnode[d]->p) == NS(w.p) ? 60 : 0;
}
