//  name:
//      merge_quirkh
//
//  purpose:
//      merge_quirkh merges two quirkh trees
//
//  parameters:
//      d               in      the depth in the QUIRKH tree
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void merge_quirkh( int d )
{
    int           a = 0, b = 0, h, i, ic, ih, k = 0, l = 0, m = 0, n = 0, ns, s, u, v, z;
    struct QUIRKD *bquirkd[NBOARDS], *bquirkdl[NBOARDS], *lquirkd[NBOARDS], *lquirkdl[NBOARDS], *nquirkd[NBOARDS], *nquirkdl[NBOARDS], *nquirkdn,
                  *vquirkd[NBOARDS], *vquirkdl[NBOARDS], *wquirkd;
    struct QUIRKH *nquirkhn, *wquirkh;
    struct TRICKS *trck;
    struct { int a, b, k, l; } stk[NBOARDS];

    u = b;
    bquirkdl[b] = &tquirkd[d];
    bquirkd[b] = bquirkdl[b]->h->d;
    while ( bquirkd[u]->d ) {
        u++;
        bquirkdl[u] = bquirkdl[u-1];
        bquirkd[u] = bquirkd[u-1]->d;
    }

    v = l;
    lquirkdl[l] = &tquirkd[d+1];
    lquirkd[l] = lquirkdl[l]->h->d;
    while ( lquirkd[v]->d ) {
        v++;
        lquirkdl[v] = lquirkdl[v-1];
        lquirkd[v] = lquirkd[v-1]->d;
    }

    i = a <= bquirkd[b]->ih && k <= lquirkd[l]->ih
        ? MIN( bquirkdl[b]->h->t[a].i, lquirkdl[l]->h->t[k].i )
        : a <= bquirkd[b]->ih ? bquirkdl[b]->h->t[a].i : lquirkdl[l]->h->t[k].i;
    ns = a <= bquirkd[b]->ih && k <= lquirkd[l]->ih && lquirkdl[l]->h->t[k].i == bquirkdl[b]->h->t[a].i
        ? MIN( lquirkdl[l]->h->t[k].ns, bquirkdl[b]->h->t[a].ns )
        : a <= bquirkd[b]->ih && bquirkdl[b]->h->t[a].i == i ? bquirkdl[b]->h->t[a].ns : lquirkdl[l]->h->t[k].ns;

    nquirkdl[n] = &tquirkd[d+2];
    nquirkdl[n]->h = nextquirkh;
    #include "bump_quirkh.c"
    nquirkdl[n]->h->d = nextquirkd;
    #include "bump_quirkd.c"
    nquirkd[n] = nquirkdl[n]->h->d;
    nquirkd[n]->ic = boardcount;
    nquirkd[n]->il = 0;
    nquirkd[n]->ih = 0;
    nquirkdl[n]->h->t[m].i = i;
    nquirkdl[n]->h->t[m].ns = ns;

loop_down:
    a = 0;
    b = 0;
    k = 0;
    l = 0;
    m = 0;
    n = 0;
    nquirkdl[n] = &tquirkd[d+2];
    nquirkd[n] = nquirkdl[n]->h->d;

    while ( YES ) {

        stk[m].a = a;
        stk[m].b = b;
        stk[m].k = k;
        stk[m].l = l;

        i = a <= bquirkd[b]->ih && k <= lquirkd[l]->ih
            ? MIN( bquirkdl[b]->h->t[a].i, lquirkdl[l]->h->t[k].i )
            : a <= bquirkd[b]->ih ? bquirkdl[b]->h->t[a].i : lquirkdl[l]->h->t[k].i;
        ns = a <= bquirkd[b]->ih && k <= lquirkd[l]->ih && lquirkdl[l]->h->t[k].i == bquirkdl[b]->h->t[a].i
            ? MIN( lquirkdl[l]->h->t[k].ns, bquirkdl[b]->h->t[a].ns )
            : a <= bquirkd[b]->ih && bquirkdl[b]->h->t[a].i == i ? bquirkdl[b]->h->t[a].ns : lquirkdl[l]->h->t[k].ns;

        #include "add_quirkh.c"

        if ( a <= bquirkd[b]->ih && bquirkdl[b]->h->t[a].i == i ) {
            a++;
            if ( a > bquirkd[b]->ih && bquirkd[b]->d )
                b++;
        }

        if ( k <= lquirkd[l]->ih && lquirkdl[l]->h->t[k].i == i ) {
            k++;
            if ( k > lquirkd[l]->ih && lquirkd[l]->d )
                l++;
        }

        if ( a > bquirkd[b]->ih && k > lquirkd[l]->ih )
            break;

        m++;
    }

    ih = m;

    while ( YES ) {

        a = stk[m].a;
        b = stk[m].b;
        k = stk[m].k;
        l = stk[m].l;

        i = nquirkdl[n]->h->t[m].i;
        if ( m == nquirkd[n]->il ) {
            if ( n ) {
                nquirkdl[n] = nquirkdl[n-1];
                nquirkd[n] = nquirkd[n-1]->d;
            }
            else {
                nquirkdl[n] = &tquirkd[d+2];
                nquirkd[n] = nquirkdl[n]->h->d;
            }
        }

        if ( k == lquirkd[l]->il && lquirkdl[l]->h->t[k].i == i ) {

            if ( lquirkd[l]->h ) {
                lquirkdl[l] = lquirkd[l];
                lquirkd[l] = lquirkdl[l]->h->d;
                v = l;
                while ( lquirkd[v]->d ) {
                    v++;
                    lquirkdl[v] = lquirkdl[v-1];
                    lquirkd[v] = lquirkd[v-1]->d;
                }
                goto loop_down;
            }

            if ( l ) {
                lquirkdl[l] = lquirkdl[l-1];
                lquirkd[l] = lquirkd[l-1]->d;
            }
            else {
                lquirkdl[l] = &tquirkd[d+1];
                lquirkd[l] = lquirkdl[l]->h->d;
            }

            v = l;
            while ( lquirkd[v]->d ) {
                v++;
                lquirkdl[v] = lquirkdl[v-1];
                lquirkd[v] = lquirkd[v-1]->d;
            }
        }

        if ( a == bquirkd[b]->il && bquirkdl[b]->h->t[a].i == i ) {

            if ( bquirkd[b]->h ) {
                bquirkdl[b] = bquirkd[b];
                bquirkd[b] = bquirkdl[b]->h->d;
                u = b;
                while ( bquirkd[u]->d ) {
                    u++;
                    bquirkdl[u] = bquirkdl[u-1];
                    bquirkd[u] = bquirkd[u-1]->d;
                }
                goto loop_down;
            }

            if ( b ) {
                bquirkdl[b] = bquirkdl[b-1];
                bquirkd[b] = bquirkd[b-1]->d;
            }
            else {
                bquirkdl[b] = &tquirkd[d];
                bquirkd[b] = bquirkdl[b]->h->d;
            }

            u = b;
            while ( bquirkd[u]->d ) {
                u++;
                bquirkdl[u] = bquirkdl[u-1];
                bquirkd[u] = bquirkd[u-1]->d;
            }
        }

        if ( ! m )
            break;

        m--;
        if ( m < nquirkd[n]->il ) {
            #include "check_quirkh.c"
            n--;
        }
    }

    delete_quirkh( d );
    delete_quirkh( d+1 );
    tquirkd[d].h = tquirkd[d+2].h;
    if ( tquirkd[d].h->d->h ) {
        n = 0;
        nquirkdl[n] = &tquirkd[d];
        nquirkd[n] = nquirkdl[n]->h->d;
        #include "check_quirkh.c"
    }
}
