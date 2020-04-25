//  name:
//      print_all_results_quirk
//
//  purpose:
//      print_all_results_quirk prints the results of the plays
//      details of the play
//
//  parameters:
//      none
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void print_all_results_quirk( void )
{
    int          d = 0, i, ix, accept, best_MAX, best_MP, best_N[NCARDS];
    double       mpscore;
    struct INDEX *tindex;
    struct NODE  *lnode[NPLIES+1];
    struct QUIRK *lquirk[NBOARDS];
    struct SCORE *lscore;

    //  initialise
    lquirk[d] = tquirk[0] ? tquirk[0]->d : NIL;

    //  loop over all nodes in the tree
    while( lquirk[d] ) {

        //  collect the info on the number of tricks
        nstricks[lquirk[d]->i] = lquirk[d]->ns;

        //  handle the sons
        if ( lquirk[d]->d ) {
            d++;
            lquirk[d] = lquirk[d-1]->d;
        }

        //  process the leaf node
        else {

            //  process the play if it is an optimum one
            if ( lquirk[d]->sc->best_MAX || lquirk[d]->sc->best_MP || lquirk[d]->sc->best_N || printallresults ) {

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

                best_MAX = lquirk[d]->sc->result > ( best_result.result - EPS );
                for ( i = 0; i <= maxns - minns; i++ ) {
                    if ( defence )
                        best_N[i] = lquirk[d]->sc->tricks[i] < ( best_result.tricks[i] + EPS ) &&
                                    lquirk[d]->sc->result > ( best_result.res[i] - EPS );
                    else
                        best_N[i] = lquirk[d]->sc->tricks[i] > ( best_result.tricks[i] - EPS ) &&
                                    lquirk[d]->sc->result > ( best_result.res[i] - EPS );
                }
                best_MP = lquirk[d]->sc->best_MP;

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
                            mpscore -= lscore->perc * lscore->mp[lquirk[d]->sc->ix];
                            lscore = lscore->r;
                        }
                    }
                    else
                        mpscore = lquirk[d]->sc->mpscore;

                    print_results( best_MAX, best_MP, best_N, mpscore );
                }

                //  delete the tree again
                delete_tree( 0, tnode );
            }

            //  get the next quirk
            while ( d && ! lquirk[d]->r )
                d--;
            lquirk[d] = lquirk[d]->r;
        }
    }
}
