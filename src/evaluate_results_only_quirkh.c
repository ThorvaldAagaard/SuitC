//  name:
//      evaluate_results_only_quirkh
//
//  purpose:
//      evaluate_results_only_quirkh evaluates the results of all plays,
//      but the probabilities for the number of tricks.
//
//  parameters:
//      rootqd          in      the root of the QUIRKH tree
//      score           out     the arrays with scores
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void evaluate_results_only_quirkh( struct QUIRKD *rootqd, struct SCORE *score )
{
    int           b, ih, ix = 0, k, l = 1;
    char          out[2*NBOARDS+1];
    struct RESULT result[NBOARDS];
    struct QUIRKD *lquirkd[NBOARDS], *lquirkdl[NBOARDS];

    //  initialise
    lquirkdl[l] = rootqd;
    lquirkd[l] = lquirkdl[l]->h ? lquirkdl[l]->h->d : NIL;
    result[0].result = 0.0;
    best_result.result = 0.0;

    //  loop over all nodes in the tree
    while( lquirkd[l] ) {

        //  compute the results
        for ( k = lquirkd[l]->il; k <= lquirkd[l]->ih; k++ ) {
            b = lquirkdl[l]->h->t[k].i;
            if ( k )
                result[k].result = result[k-1].result + prob[b] * dupl[b] * lquirkdl[l]->h->t[k].ns;
            else
                result[k].result = prob[b] * dupl[b] * lquirkdl[l]->h->t[k].ns;
        }

        //  get the next distribution
        if ( lquirkd[l]->d ) {
            l++;
            lquirkdl[l] = lquirkdl[l-1];
            lquirkd[l] = lquirkd[l-1]->d;
        }

        //  we reached a terminal node; process the result
        else {

            //  determine the best result
            ih = lquirkd[l]->ih;
            if ( best_result.result < result[ih].result )
                best_result.result = result[ih].result;

            //  store the score
            lquirkdl[l]->h->sc = &score[ix];
            score[ix].best_MAX = YES;
            score[ix].best_MP = ( matchpoint ? YES : NO );
            score[ix].best_N = YES;
            score[ix].ix = ix;
            score[ix].result = result[ih].result;
            score[ix].tricks = NIL;
            score[ix].mp = NIL;
            ix++;

            //  get the next distribution
            while ( l > 1 && ! lquirkd[l]->h )
                l--;
            lquirkdl[l] = lquirkd[l];
            lquirkd[l] = lquirkdl[l]->h ? lquirkdl[l]->h->d : NIL;
        }
    }
}
