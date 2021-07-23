//  name:
//      build_scard
//
//  purpose:
//      build_scard builds a list of SCARD sequences
//
//  parameters:
//      s               in      the suit that is analysed
//      c               in      the cards of the four players

#include "header.h"

struct SCARD *build_scard( int s, COLOR c[4] )
{
    int           i, p;
    struct SCARD  *lc, zscard;

    //  convert the cards into a SCARD list
    lc = &zscard;
    zscard.d = NIL;
    zscard.p = unknown;
    for ( i = ace; i >= two; i-- ) {

        for ( p = 0; p < 4; p++ ) {
            if ( c[p] & set(i) ) {
                if ( lc->p == p ) {
                    lc->n++;
                    lc->c |= set(i);
                }
                else {
                    bump_scard( lc->d );
                    lc = lc->d;
                    lc->n = 1;
                    lc->p = p;
                    lc->s = s;
                    lc->c = set(i);
                }
            }
        }
    }

    return( zscard.d );
}
