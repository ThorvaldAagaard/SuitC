//  name:
//      build_node
//
//  purpose:
//      build_node builds a NODE entry
//
//  parameters:
//      d               in      the depth in the tree
//      s               in      the suit of this node
//      p               in      the player of this node
//      c               in      the cards of this node

//  #include "header.h"

void build_node( int d, int s, int p, COLOR c )
{
    int           b, dwin, lds, q, t = st[d].t;
    struct        { int p, s; COLOR c; } w;

    dwin = lnode[d-1]->dwin;
    dwin = st[d].ld ? d : s == lnode[dwin]->s && c > lnode[dwin]->c || s == trump && lnode[dwin]->s != trump ? d : dwin;

    //  create a new NODE entry
    ncount++;
    ncalls[0]++;
    lnode[d]->r = nextnode;
    if ( ncount > ncountmax ) {
        ncountmax++;
        nextnode++;
    }
    else
        nextnode = nextnode->r;
    lnode[d] = lnode[d]->r;
    lnode[d]->d = NIL;
    lnode[d]->r = NIL;
    lnode[d]->p = p;
    lnode[d]->s = s;
    lnode[d]->c = c;
    lnode[d]->nc = st[d].la && NS(lnode[dwin]->p) ? 1 : 0;
    lnode[d]->nd = lnode[d-1]->nc + lnode[d-1]->nd;
    lnode[d]->nh = ntricks - d/4;
    lnode[d]->nl = 0;
    lnode[d]->dwin = dwin;
    lnode[d]->w = lnode[dwin]->p;
    lnode[d]->t = d == 4*ntricks ? YES : NO;

    w.p = lnode[dwin]->p;
    w.s = lnode[dwin]->s;
    w.c = lnode[dwin]->c;
    lds = lnode[st[d].l]->s;

    for ( b = d+1; b <= st[d].l+3; b++ ) {
        q = (p+b-d)%4;
        if ( w.s == lds && brd[t][lds][q] > w.c )
            { w.p = q; w.s = lds; w.c = brd[t][lds][q]; }
        if ( ! brd[t][lds][q] && ( w.s == trump && brd[t][trump][q] > w.c || w.s != trump && brd[t][trump][q] ) )
            { w.p = q; w.s = trump; w.c = brd[t][trump][q]; }
    }

    lnode[d]->qt = NS(lnode[d]->p) == NS(w.p) ? 1 : 0;
}
