//  name:
//      print_all_results
//
//  purpose:
//      print_all_results prints the results of the plays
//
//  parameters:
//      none
//
//  Copyright 2024, Gerrit A. van der Velde, Leiden

#include "header.h"

void print_all_results( void )
{
    int           accept, best_MAX, best_MP, best_N[NCARDS], i, ih, ix = 0, k, l = 0;
    double        mpscore, tricksum;
    struct INDEX  *tindex;
    struct NODE   *lnode[NPLIES+1];
    struct QUIRKD *lquirkd[NBOARDS], *lquirkdl[NBOARDS];
    struct RESULT result[NBOARDS];
    struct SCORE  *lscore;

    //  initialise
    lquirkdl[0] = quirkd;
    lquirkd[0] = lquirkdl[0]->h ? lquirkdl[0]->h->d : NIL;

    //  loop over all nodes in the tree
    while( lquirkd[l] ) {

        //  collect the info on the number of tricks
        for ( k = lquirkd[l]->il; k <= lquirkd[l]->ih; k++ )
             nstricks[lquirkdl[l]->h->t[k].i] = lquirkdl[l]->h->t[k].ns;

        //  compute the results
        for ( k = lquirkd[l]->il; k <= lquirkd[l]->ih; k++ ) {
            if ( k ) {
                result[k].result = result[k-1].result + prob[k] * dupl[k] * lquirkdl[l]->h->t[k].ns;
                for ( i = minns; i <= maxns; i++ )
                    result[k].tricks[i] = result[k-1].tricks[i];
                result[k].tricks[lquirkdl[l]->h->t[k].ns] += prob[k] * dupl[k];
            }
            else {
                result[k].result = prob[k] * dupl[k] * lquirkdl[l]->h->t[k].ns;
                for ( i = minns; i <= maxns; i++ )
                    result[k].tricks[i] = 0.0;
                result[k].tricks[lquirkdl[l]->h->t[k].ns] = prob[k] * dupl[k];
            }
        }

        //  get the oldest son
        if ( lquirkd[l]->d ) {
            l++;
            lquirkdl[l] = lquirkdl[l-1];
            lquirkd[l] = lquirkd[l-1]->d;
        }

        //  we reached a leaf
        else {

            //  check for an optimal play
            ix++;
            ih = lquirkd[l]->ih;
            tricksum = 0.0;
            best_MAX = result[ih].result > best_result.result - EPS;
            best_MP = matchpoint ? score[ix].best_MP : NO;
            accept = best_MAX | best_MP;
            if ( defence ) {
                for ( i = minns; i <= maxns; i++ ) {
                    tricksum += result[ih].tricks[i];
                    best_N[i] = tricksum < best_result.tricks[i] + EPS && result[ih].result > best_result.res[i] - EPS;
                    accept |= best_N[i];
                }
            }
            else {
                for ( i = maxns; i >= minns; i-- ) {
                    tricksum += result[ih].tricks[i];
                    best_N[i] = tricksum > best_result.tricks[i] - EPS && result[ih].result > best_result.res[i] - EPS;
                    accept |= best_N[i];
                }
            }

            //  restrict the number of plays that is reported
            if ( accept && SUITCAPP ) {
                for ( k = 0; k < nbest_plays; k++ ) {
                    int match = YES, l;
                    for ( i = minns; i <= maxns; i++ )
                        if ( best_play[k].tricks[i] < result[ih].tricks[i] - EPS || best_play[k].tricks[i] > result[ih].tricks[i] + EPS )
                            match = NO;
                    if ( match ) {
                        if ( best_play[k].dupl )
                            best_play[k].dupl--;
                        else
                            accept = NO;
                        break;
                    }
                }
            }

            //  print the results of this play
            if ( accept || printallresults ) {

                //  print the results
                if ( matchpoint ) {
                    mpscore = 0.0;
                    lscore = score[0].r;
                    while ( lscore->r ) {
                        mpscore -= 100. * lscore->perc * lscore->mp[score[ix].ix];
                        lscore = lscore->r;
                    }
                }
                else
                    mpscore = 0.0;
                print_results( best_MAX, best_MP, best_N, mpscore );

                //  create and print a node tree for this play
                if ( ! resultsonly ) {

                    //  create the tree
                    build_selected_tree();

                    //  create the tree
                    if ( maxply && compacttree == NO )
                        select_ew_plays( 1 );

                    //  merge identical plays and select the preferred ones
                    if ( maxply && mergeselect & 0x1 )
                        merge_tree( 1, west );
                    if ( maxply && mergeselect & 0x2 )
                        merge_tree( 1, north );
                    if ( maxply && mergeselect & 0x4 )
                        select_ns_plays( 1 );

                    //  print the tree
                    printf( "\n" );
                    if ( tnode[0]->d )
                        print_tree( 1, tnode[0]->d, YES, YES );
                    else
                        print_tree( 0, tnode[0], YES, YES );

                    //  delete the tree again
                    delete_tree( 0, tnode );
                }
            }

            //  get the next sibling
            while ( l && lquirkd[l]->h == NIL )
                l--;
            lquirkdl[l] = lquirkd[l];
            lquirkd[l] = lquirkd[l]->h ? lquirkd[l]->h->d : NIL;
        }
    }
}
