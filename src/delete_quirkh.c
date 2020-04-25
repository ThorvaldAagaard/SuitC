//  name:
//      delete_quirkh
//
//  purpose:
//      delete_quirkh deletes a QUIRKH tree
//
//  parameters:
//      d               in      the depth in the tree
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void delete_quirkh( int d )
{
    int           l = 0;
    struct QUIRKD *lquirkd[NBOARDS];

    lquirkd[l] = tquirkd[d].h->d;
    while ( tquirkd[d].h ) {

        if ( lquirkd[l]->d ) {
            l++;
            lquirkd[l] = lquirkd[l-1]->d;
            if ( lquirkd[l]->h == NIL ) {
                printf( "\n\tintegrity failure in the QUIRK tree\n" );
                exit( 60 );
            }
        }

        else if ( l == 0 ) {

            qhcount--;
            tquirkd[d].h->h = nextquirkh;
            nextquirkh = tquirkd[d].h;
            tquirkd[d].h = lquirkd[l]->h;

            qdcount--;
            lquirkd[l]->d = nextquirkd;
            nextquirkd = lquirkd[l];

            if ( tquirkd[d].h )
                lquirkd[l] = tquirkd[d].h->d;
        }

        else if ( l && lquirkd[l] == lquirkd[l-1]->d && lquirkd[l]->h ) {

            lquirkd[l-1]->d = lquirkd[l]->h->d;

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
        }

        else {
            printf( "\n\tintegrity failure in the QUIRK tree\n" );
            exit( 61 );
        }
    }
}
