//  name:
//      fold_quirkh
//
//  purpose:
//      fold_quirkh folds two QUIRKH trees
//
//  parameters:
//      d               in      the depth in the NODE tree
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void fold_quirkh( int d )
{
    int           h, i, ic, ih, k = 0, l = 0, m = 0, n = 0, ns, s, u, v = 0, z;
    struct QUIRKD *nquirkd[NBOARDS], *lquirkd[NBOARDS], *lquirkdl[NBOARDS], *nquirkdl[NBOARDS], *nquirkdn,
                  *vquirkd[NBOARDS], *vquirkdl[NBOARDS], *wquirkd;
    struct QUIRKH *nquirkhn, *wquirkh;
    struct TRICKS *trck;

    lquirkdl[l] = &tquirkd[d+1];
    lquirkd[l] = lquirkdl[v]->h->d;
    while ( lquirkd[v]->d ) {
        v++;
        lquirkd[v] = lquirkd[v-1]->d;
    }

    ih = lquirkd[v]->ih;
    nquirkdl[n] = &tquirkd[d];
    nquirkd[n] = nquirkdl[n]->h->d;

    while ( YES ) {

        while ( k <= lquirkd[l]->ih ) {

            i = lquirkdl[l]->h->t[k].i;
            ns = lquirkdl[l]->h->t[k].ns;
            #include "add_quirkh.c"

            if ( k <= lquirkd[l]->ih ) {
                k++;
                if ( k > lquirkd[l]->ih && lquirkd[l]->d ) {
                    l++;
                    lquirkdl[l] = lquirkdl[l-1];
                    lquirkd[l] = lquirkd[l-1]->d;
                }
            }

            if ( k > lquirkd[l]->ih && ! lquirkd[l]->h )
                break;

            m++;
        }

        while ( l && ! lquirkd[l]->h )
            l--;

        k = lquirkd[l]->il;
        i = lquirkdl[l]->h->t[k].i;

        while ( nquirkdl[n]->h->t[nquirkd[n]->il].i > i ) {
            nquirkdl[n] = nquirkdl[n-1];
            nquirkd[n] = nquirkd[n-1]->d;
            #include "check_quirkh.c"
            n--;
        }

        m = nquirkd[n]->ih;
        while ( nquirkdl[n]->h->t[m].i > i )
            m--;

        if ( ! lquirkd[l]->h )
            break;

        lquirkdl[l] = lquirkd[l];
        lquirkd[l] = lquirkdl[l]->h->d;
    }

    delete_quirkh( d+1 );
    if ( tquirkd[d].h->d->h ) {
        n = 0;
        nquirkdl[n] = &tquirkd[d];
        nquirkd[n] = nquirkdl[n]->h->d;
        #include "check_quirkh.c"
    }
}
