//  name:
//      evaluate_all_results_quirkh
//
//  purpose:
//      evaluate_all_results_quirkh evaluates the results of all plays using the QUIRKH tree
//
//  parameters:
//      none
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void evaluate_all_results_quirkh( void )
{
    int           d = 0, i, ix, j, k, l = 0, n;
    char          out[2*NBOARDS+1];
    static char   hexdigit[] = "0123456789abcdef";
    struct RESULT result[NBOARDS];
    struct QUIRKD *lquirkd[NBOARDS];
    struct QUIRKH *lquirkh[NBOARDS];

    //  print the header for the list of all results
    if ( printsummary ) {
        printf( "\n\n\nThe complete list of play theoretic acceptable results\n"   );
        printf(       "======================================================\n\n" );
        if ( DUALHEAD ) {
            for ( i = z_count-1; i >= 0; i-- ) {
                for ( j = 0; j < boardcount; j++ )
                    printf( " %c", b_print[i][j] );
                printf( "\n" );
            }
        }
        else {
            for ( i = ew_c_count-1; i >= 0; i-- ) {
                for ( j = 0; j < b_count[i]; j++ )
                    printf( " %c", b_print[i][j] );
                printf( "\n" );
            }
        }
        for ( i = 0; i < b_count[0]; i++ )
            printf( "--" );
        if ( defence )
            for ( i = maxns - minns; i >= 0; i-- )
                printf( "  -- %x --", tricklimit - i - minns );
        else
            for ( i = 0; i <= maxns - minns; i++ )
                printf( "  -- %x --", i + minns );
        printf( "  ++++++\n" );
    }

    //  create the links for the SCORE entries
    score = (struct SCORE *) malloc( ( nplays + 2 ) * sizeof( struct SCORE ) );
    memory += ( nplays + 2 ) * sizeof( struct SCORE );
    if ( ! score ) {
        printf( "\nERROR: No more memory available: %6.1f MB after %3.1f seconds\n" , (float) memory / 1048576, TICK * clock() );
        exit( 1 );
    }
    score[0].l = NIL;
    for ( i = 0; i <= nplays; i++ ) {
        score[i+1].l = &score[i];
        score[i].r = &score[i+1];
        score[i].b = NIL;
        score[i].ix = i-1;
        score[i].mp = NIL;
        score[i].mpscore = 0.0;
    }
    score[nplays+1].r = NIL;

    //  reserve space to store the results per trick
    score[1].tricks = (double *) malloc( nplays * ( maxns - minns + 1 ) * sizeof( double ) );
    memory += nplays * ( maxns - minns + 1 ) * sizeof( double );
    if ( ! score[1].tricks ) {
        printf( "\nERROR: No more memory available: %6.1f MB after %3.1f seconds\n" , (float) memory / 1048576, TICK * clock() );
        exit( 1 );
    }
    for ( i = 2; i <= nplays; i++ )
        score[i].tricks = score[i-1].tricks + ( maxns - minns + 1 );

    //  initialise
    out[0] = '\0';
    if ( tquirkd[0].h ) {
        lquirkh[d] = tquirkd[0].h;
        lquirkd[d] = lquirkh[d]->d;
    }
    else {
        lquirkh[d] = NIL;
        lquirkd[d] = NIL;
    }

    //  initialize the results
    best_result.result = 0.0;
    for ( i = 0; i <= maxns - minns; i++ ) {
        best_result.tricks[i] = defence ? 1.0 : 0.0;
        best_result.res[i] = 0.0;
    }

    //  loop over all quirks in the tree
    ix = 0;
    while( lquirkd[d] ) {

        //  compute the results
        for ( k = lquirkd[d]->il; k <= lquirkd[d]->ih; k++ ) {
            if ( k ) {
                result[k].result = result[k-1].result + prob[k] * dupl[k] * lquirkh[d]->t[k].ns;
                for ( i = 0; i <= maxns - minns; i++ )
                    result[k].tricks[i] = result[k-1].tricks[i];
                result[k].tricks[lquirkh[d]->t[k].ns-minns] += prob[k] * dupl[k];
            }
            else {
                result[k].result = prob[k] * dupl[k] * lquirkh[d]->t[k].ns;
                for ( i = 0; i <= maxns - minns; i++ )
                    result[k].tricks[i] = 0.0;
                result[k].tricks[lquirkh[d]->t[k].ns-minns] = prob[k] * dupl[k];
            }

            //  build the output string
            if ( printsummary ) {
                for ( i = l; i < k; i++) {
                    out[l++] = ' ';
                    out[l++] = '-';
                }
                out[l++] = ' ';
                out[l++] = ( defence ? hexdigit[tricklimit-lquirkh[d]->t[k].ns] : hexdigit[lquirkh[d]->t[k].ns] );
            }
        }

        //  get the next distribution
        if ( lquirkd[d]->d ) {
            d++;
            lquirkh[d] = lquirkh[d-1];
            lquirkd[d] = lquirkd[d-1]->d;
        }

        //  we reached a leaf; process the result
        else {

            //  print the result
            if ( printsummary ) {
                out[l] = '\0';
                printf( "%s", out );
                if ( BLANK )
                    for ( n = 0; n < l; n++ )
                        out[n] = ' ';
                if ( defence ) {
                    for ( i = maxns - minns; i >= 0; i-- )
                        printf( "%9.4f", 100.0 * result[lquirkd[d]->ih].tricks[i] );
                    printf( "%8.4f\n", tricklimit - result[lquirkd[d]->ih].result );
                }
                else {
                    for ( i = 0; i <= maxns - minns; i++ )
                        printf( "%9.4f", 100.0 * result[lquirkd[d]->ih].tricks[i] );
                    printf( "%8.4f\n", result[lquirkd[d]->ih].result );
                }
            }

            //  determine the best result
            if ( best_result.result < result[lquirkd[d]->ih].result )
                best_result.result = result[lquirkd[d]->ih].result;
            if ( defence )
                for ( i = 0; i < maxns - minns; i++ )
                    result[lquirkd[d]->ih].tricks[i+1] += result[lquirkd[d]->ih].tricks[i];
            else
                for ( i = maxns - minns; i > 0; i-- )
                    result[lquirkd[d]->ih].tricks[i-1] += result[lquirkd[d]->ih].tricks[i];
            if ( defence ) {
                for ( i = 0; i <= maxns - minns; i++ ) {
                    if ( best_result.tricks[i] > ( result[lquirkd[d]->ih].tricks[i] + EPS ) || (
                         best_result.tricks[i] > ( result[lquirkd[d]->ih].tricks[i] - EPS ) &&
                         best_result.res[i] < ( result[lquirkd[d]->ih].result + EPS ) ) ) {
                        best_result.tricks[i] = result[lquirkd[d]->ih].tricks[i];
                        best_result.res[i] = result[lquirkd[d]->ih].result;
                    }
                }
            }
            else {
                for ( i = 0; i <= maxns - minns; i++ ) {
                    if ( best_result.tricks[i] < ( result[lquirkd[d]->ih].tricks[i] - EPS ) || (
                         best_result.tricks[i] < ( result[lquirkd[d]->ih].tricks[i] + EPS ) &&
                         best_result.res[i] < ( result[lquirkd[d]->ih].result + EPS ) ) ) {
                        best_result.tricks[i] = result[lquirkd[d]->ih].tricks[i];
                        best_result.res[i] = result[lquirkd[d]->ih].result;
                    }
                }
            }

            //  store the score
            lquirkh[d]->sc = &score[++ix];
            score[ix].ix = ix - 1;
            score[ix].result = result[lquirkd[d]->ih].result;
            for ( i = 0; i <= maxns - minns; i++ )
                score[ix].tricks[i] = result[lquirkd[d]->ih].tricks[i];
            score[ix].mp = NIL;

            //  get the next distribution
            d++;
            lquirkh[d] = lquirkh[d-1];
            lquirkd[d] = lquirkd[d-1]->d;
            while ( d && ! lquirkd[d] ) {
                d--;
                lquirkh[d] = lquirkd[d]->h;
                lquirkd[d] = lquirkh[d] ? lquirkh[d]->d : NIL;
            }
            l = lquirkd[d] ? 2 * lquirkd[d]->il : 0;
            out[l] = '\0';
        }
    }

    //  set the best_{MAX|MP|N} flags for each play
    for ( ix = 1; ix <= nplays; ix++ ) {
        score[ix].best_MAX = score[ix].result > ( best_result.result - EPS );
        score[ix].best_MP = ( impscore || matchpoint ? YES : NO );
        score[ix].best_N = NO;
        for ( i = 0; i <= maxns - minns; i++ )
            score[ix].best_N |= ( score[ix].tricks[i] > ( best_result.tricks[i] - EPS ) ) &&
                                ( score[ix].result    > ( best_result.res[i]    - EPS ) );
    }
}
