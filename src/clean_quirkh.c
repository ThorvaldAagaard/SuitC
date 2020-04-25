//  name:
//      clean_quirkh
//
//  purpose:
//      clean_quirkh removes all quirks that cannot result in the best play
//
//  parameters:
//      rootqd           in      the root of the QUIRKH tree
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

int clean_quirkh( struct QUIRKD *rootqd )
{
    int           k, l = 0;
    struct QUIRKD *lquirkd[NBOARDS], *lquirkdl[NBOARDS];

    //  initialise
    nplays = 0;
    lquirkdl[l] = rootqd;
    lquirkd[l] = lquirkdl[l]->h->d;

    while ( lquirkd[l] ) {

        if ( lquirkd[l]->d ) {
            l++;
            lquirkdl[l] = lquirkdl[l-1];
            lquirkd[l] = lquirkd[l-1]->d;
        }

        else {

            if ( lquirkdl[l]->h->sc->best_MAX || lquirkdl[l]->h->sc->best_MP || lquirkdl[l]->h->sc->best_N ) ;

            else {

                if ( lquirkd[l] == lquirkdl[l]->h->d ) {

                    qhcount--;
                    lquirkdl[l]->h->h = nextquirkh;
                    nextquirkh = lquirkdl[l]->h;
                    lquirkdl[l]->h = lquirkd[l]->h;

                    qdcount--;
                    lquirkd[l]->d = nextquirkd;
                    nextquirkd = lquirkd[l];
                    lquirkd[l] = lquirkdl[l];

                    if ( l && lquirkd[l]->h == NIL && lquirkd[l] == lquirkd[l-1]->d ) {

                        lquirkd[l-1]->d = lquirkdl[l]->d;
                        lquirkd[l-1]->ih = lquirkdl[l]->ih;
                        qdcount--;
                        lquirkdl[l]->d = nextquirkd;
                        nextquirkd = lquirkdl[l];
                        l--;
                    }
                }

                else if ( l && lquirkd[l] == lquirkd[l-1]->d ) {

                    lquirkd[l-1]->d = lquirkd[l]->h->d;
                    lquirkdl[l]->h->sc = lquirkd[l]->h->sc;
                    for ( k = lquirkd[l]->il; k <= lquirkd[l]->ih; k++ )
                        lquirkdl[l]->h->t[k] = lquirkd[l]->h->t[k];

                    qhcount--;
                    lquirkd[l]->h->h = nextquirkh;
                    nextquirkh = lquirkd[l]->h;

                    qdcount--;
                    lquirkd[l]->d = nextquirkd;
                    nextquirkd = lquirkd[l];
                    lquirkd[l] = lquirkd[l-1]->d;

                    if ( lquirkd[l]->h == NIL ) {
                        lquirkd[l-1]->d = lquirkd[l]->d;
                        lquirkd[l-1]->ih = lquirkd[l]->ih;
                        qdcount--;
                        lquirkd[l]->d = nextquirkd;
                        nextquirkd = lquirkd[l];
                        l--;
                    }

                    continue;
                }
            }

            while ( l && lquirkd[l]->h == NIL )
                l--;
            lquirkdl[l] = lquirkd[l];
            lquirkd[l] = lquirkd[l]->h ? lquirkd[l]->h->d : NIL;
        }
    }
}
