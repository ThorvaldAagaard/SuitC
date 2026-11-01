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
//  Copyright 2026, Gerrit A. van der Velde, Leiden

#include "header.h"

void build_node( int d, int b, int s, int p, COLOR c )
{
    int           l, lds, q, t = st[d].t;
    struct        { int p, s; COLOR c; } w;

    //  create the new NODE
    ncount++;
    wnode[d]->r = nextnode;
    wnode[d] = wnode[d]->r;
    if ( ncountmax < ncount ) {
        ncountmax++;
        nextnode++;
        if ( ncountmax == nodemax ) {
            printf( "No more memory for the NODE entries; increase the value of NNODE\n" );
            print_stat( 'P' );
        }
    }
    else
        nextnode = nextnode->r;
    wnode[d]->d = NIL;
    wnode[d]->r = NIL;

    if ( st[d].ld || s == wnode[d-1]->w_s && c > wnode[d-1]->w_c || s == trump && wnode[d-1]->w_s != trump ) {
        wnode[d]->w_p = p;
        wnode[d]->w_s = s;
        wnode[d]->w_c = c;
    }
    else {
        wnode[d]->w_p = wnode[d-1]->w_p;
        wnode[d]->w_c = wnode[d-1]->w_c;
        wnode[d]->w_s = wnode[d-1]->w_s;
    }

    wnode[d]->p = p;
    wnode[d]->s = s;
    wnode[d]->c = c;
    wnode[d]->nc = st[d].la && NS(wnode[d]->w_p) ? 1 : 0;
    wnode[d]->nd = wnode[d-1]->nc + wnode[d-1]->nd;
    wnode[d]->nh = ntricks - d/4;
    wnode[d]->nl = 0;
    wnode[d]->t = d == 4*ntricks ? YES : NO;

    w.p = wnode[d]->w_p;
    w.s = wnode[d]->w_s;
    w.c = wnode[d]->w_c;
    lds = wnode[st[d].l]->s;

    for ( l = d+1; l <= st[d].l+3; l++ ) {
        q = (p+l-d)%4;
        if ( w.s == lds && scard[t][b][lds]->brd[q] > w.c ) {
            w.p = q;
            w.s = lds;
            w.c = scard[t][b][lds]->brd[q];
        }
        if ( ! scard[t][b][lds]->brd[q] &&
             ( w.s == trump && scard[t][b][trump]->brd[q] > w.c ||
               trump != notrump && w.s != trump && scard[t][b][trump]->brd[q] ) ) {
            w.p = q;
            w.s = trump;
            w.c = scard[t][b][trump]->brd[q];
        }
    }

    wnode[d]->rnk = NS(wnode[d]->p) == NS(w.p) ? 60 : 0;
}
