//  name:
//      print_all_results_quirkh
//
//  purpose:
//      print_all_results_quirkh prints the results of the plays
//      details of the play
//
//  parameters:
//      none
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void print_all_results_quirkh( void )
{
    int           d = 0, i, ix, k, accept, best_MAX, best_MP, best_N[NCARDS];
    double        mpscore;
    struct NODE  *lnode[NPLIES+1];
    struct INDEX  *tindex;
    struct QUIRKD *lquirkd[NBOARDS+1];
    struct QUIRKH *lquirkh[NBOARDS+1];
    struct SCORE *lscore;

    //  initialise
    lquirkh[d] = tquirkd[d].h;
    lquirkd[d] = lquirkh[d] ? lquirkh[d]->d : NIL;

    //  loop over all nodes in the tree
    while( lquirkd[d] ) {

        //  collect the info on the number of tricks
        for ( k = lquirkd[d]->il; k <= lquirkd[d]->ih; k++ )
             nstricks[lquirkh[d]->t[k].i] = lquirkh[d]->t[k].ns;

        //  handle the sons
        if ( lquirkd[d]->d ) {
            d++;
            lquirkh[d] = lquirkh[d-1];
            lquirkd[d] = lquirkd[d-1]->d;
        }

        //  process the leaf node
        else {

            //  process the play if it is an optimum one
            if ( lquirkh[d]->sc->best_MAX || lquirkh[d]->sc->best_MP || lquirkh[d]->sc->best_N || printallresults ) {

                //  create a node tree for this play
                build_selected_tree();

                //  check the correctness of the tree
                tnode[1] = tnode[0]->d;
                while ( tnode[1] ) {
                    tindex = tnode[1]->i;
                    while ( tindex ) {
                        i = tindex->i;
                        if ( ( tindex->max != nstricks[i] ) || ( tindex->min != nstricks[i] ) ) {
                            printf( "\n\tERROR:" );
                            printf( " c=%04x", tnode[1]->c );
                            printf( " b=%03d", i );
                            printf( " min=%d", ( defence ? tricklimit - tindex->max : tindex->min ) );
                            printf( " max=%d", ( defence ? tricklimit - tindex->min : tindex->max ) );
                            printf( " ns=%d\n", ( defence ? tricklimit - nstricks[i] : nstricks[i] ) );
                        }
                        tindex = tindex->r;
                    }
                    tnode[1] = tnode[1]->r;
                }

                best_MAX = lquirkh[d]->sc->result > ( best_result.result - EPS );
                for ( i = 0; i <= maxns - minns; i++ ) {
                    if ( defence )
                        best_N[i] = lquirkh[d]->sc->tricks[i] < ( best_result.tricks[i] + EPS ) &&
                                    lquirkh[d]->sc->result > ( best_result.res[i] - EPS );
                    else
                        best_N[i] = lquirkh[d]->sc->tricks[i] > ( best_result.tricks[i] - EPS ) &&
                                    lquirkh[d]->sc->result > ( best_result.res[i] - EPS );
                }
                best_MP = lquirkh[d]->sc->best_MP;

                //  check if any optimal play is left
                accept = NO;
                if ( best_MAX || best_MP )
                    accept = YES;
                for ( i = 0; i <= maxns - minns; i++ )
                    if ( best_N[i] )
                        accept = YES;

                //  print the results
                if ( accept || printallresults ) {

                    //  merge identical plays and select the preferred ones
                    if ( mergeselect && maxply ) {
                        if ( mergeselect & 0x1 )
                            merge_tree( 1 );
                        if ( mergeselect & 0x2 )
                            select_ns_plays( 1 );
                    }

                    //  print the results
                    if ( matchpoint && best_MAX && ! best_MP ) {
                        mpscore = 0.0;
                        lscore = score[0].r;
                        while ( lscore->r ) {
                            mpscore -= lscore->perc * lscore->mp[lquirkh[d]->sc->ix];
                            lscore = lscore->r;
                        }
                    }
                    else
                        mpscore = lquirkh[d]->sc->mpscore;

                    print_results( best_MAX, best_MP, best_N, mpscore );
                }

                //  delete the tree again
                delete_tree( 0, tnode );
            }

            //  get the next quirk
            while ( d && ! lquirkd[d]->h )
                d--;
            lquirkh[d] = lquirkd[d]->h;
            lquirkd[d] = lquirkh[d] ? lquirkh[d]->d : NIL;
        }
    }
}
