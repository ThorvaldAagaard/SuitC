//  name:
//      evaluate_results_only_quirk
//
//  purpose:
//      evaluate_results_only_quirk evaluates the results of all plays,
//      but the probabilities for the number of tricks.
//
//  parameters:
//      rootq           in      the root of the quirk tree
//      score           out     the arrays with scores
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void evaluate_results_only_quirk( struct QUIRK *rootq, struct SCORE *score )
{
    int           b, d, i, ix, j, l;
    char          out[2*NBOARDS+1];
    struct RESULT result[NBOARDS+2];
    struct QUIRK  *lquirk[NBOARDS+2];

    //  initialise
    d = 1;
    ix = 0;
    lquirk[d] = rootq;
    if ( lquirk[d]->i < 0 )
        lquirk[d] = lquirk[d]->d;
    result[0].result = 0.0;
    best_result.result = 0.0;

    //  loop over all nodes in the tree
    while( lquirk[d] ) {

        //  compute the results
        b = lquirk[d]->i;
        result[d].result = result[d-1].result + prob[b] * dupl[b] * lquirk[d]->ns;

        //  get the next distribution
        if ( lquirk[d]->d ) {
            d++;
            lquirk[d] = lquirk[d-1]->d;
        }

        //  we reached a terminal node; process the result
        else {

            //  determine the best result
            if ( best_result.result < result[d].result )
                best_result.result = result[d].result;

            //  store the score
            lquirk[d]->sc = &score[ix];
            score[ix].best_MAX = YES;
            score[ix].best_MP = ( matchpoint ? YES : NO );
            score[ix].best_N = YES;
            score[ix].ix = ix;
            score[ix].result = result[d].result;
            score[ix].tricks = NIL;
            score[ix].mp = NIL;
            ix++;

            //  get the next distribution
            while ( ( d > 1 ) && ! lquirk[d]->r )
                d--;
            lquirk[d] = lquirk[d]->r;
        }
    }
}
