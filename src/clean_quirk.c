//  name:
//      clean_quirk
//
//  purpose:
//      clean_quirk removes all quirks that cannot result in the best play
//
//  parameters:
//      rootq           in      the source quirk
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void clean_quirk( struct QUIRK *rootq )
{
    int          d;
    struct QUIRK *aquirk, *lquirk[NBOARDS+1];

    //  initialise
    d = 1;
    lquirk[0] = rootq;
    lquirk[1] = lquirk[0]->d;

    //  loop over all nodes in the tree
    while( lquirk[d] ) {

        //  handle the sons first
        if ( lquirk[d]->d ) {
            d++;
            lquirk[d] = lquirk[d-1]->d;
        }

        //  get the next quirk
        else {

            //  handle a terminal quirk
            if ( lquirk[d]->sc ) {

                //  delete the quirk if it cannot result in the best play
                if ( ( lquirk[d]->sc->best_MAX || lquirk[d]->sc->best_MP || lquirk[d]->sc->best_N ) )
                    lquirk[d] = lquirk[d]->r;
                else {
                    if ( lquirk[d-1]->d == lquirk[d] )
                        lquirk[d-1]->d = lquirk[d]->r;
                    else {
                        aquirk = lquirk[d-1]->d;
                        while ( aquirk->r != lquirk[d] )
                            aquirk = aquirk->r;
                        aquirk->r = lquirk[d]->r;
                    }
                    aquirk = lquirk[d]->r;
                    lquirk[d]->r = nextquirk;
                    nextquirk = lquirk[d];
                    qcount--;
                    lquirk[d] = aquirk;
                }

                //  delete quirks without sons
                while ( ( d > 1 ) && ! lquirk[d] ) {
                    d--;
                    if ( lquirk[d]->d )
                        lquirk[d] = lquirk[d]->r;
                    else {
                        if ( lquirk[d-1]->d == lquirk[d] )
                            lquirk[d-1]->d = lquirk[d]->r;
                        else {
                            aquirk = lquirk[d-1]->d;
                            while ( aquirk->r != lquirk[d] )
                                aquirk = aquirk->r;
                            aquirk->r = lquirk[d]->r;
                        }
                        aquirk = lquirk[d]->r;
                        lquirk[d]->r = nextquirk;
                        nextquirk = lquirk[d];
                        qcount--;
                        lquirk[d] = aquirk;
                    }
                }
            }

            //  get the next quirk
            else {
                while ( ( d > 1 ) && ! lquirk[d]->r )
                    d--;
                lquirk[d] = lquirk[d]->r;
            }
        }
    }
}
