//  name:
//      evaluate_all_results_quirk
//
//  purpose:
//      evaluate_all_results_quirk evaluates the results of all plays
//
//  parameters:
//      none
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void evaluate_all_results_quirk( void )
{
    int           d = 0, i, ix, j, l = 0, n;
    char          out[2*NBOARDS+1];
    static char   hexdigit[] = "0123456789abcdef";
    struct RESULT result[NBOARDS];
    struct QUIRK  *lquirk[NBOARDS];

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
    lquirk[d] = tquirk[0] ? tquirk[0]->d : NIL;

    //  initialize the results
    best_result.result = 0.0;
    for ( i = 0; i <= maxns - minns; i++ ) {
        best_result.tricks[i] = defence ? 1.0 : 0.0;
        best_result.res[i] = 0.0;
    }

    //  loop over all quirks in the tree
    ix = 0;
    while( lquirk[d] ) {

        //  compute the results
        if ( d ) {
            result[d].result = result[d-1].result + prob[d] * dupl[d] * lquirk[d]->ns;
            for ( i = 0; i <= maxns - minns; i++ )
                result[d].tricks[i] = result[d-1].tricks[i];
            result[d].tricks[lquirk[d]->ns-minns] += prob[d] * dupl[d];
        }
        else {
            result[d].result = prob[d] * dupl[d] * lquirk[d]->ns;
            for ( i = 0; i <= maxns - minns; i++ )
                result[d].tricks[i] = 0.0;
            result[d].tricks[lquirk[d]->ns-minns] = prob[d] * dupl[d];
        }

        //  build the output string
        if ( printsummary ) {
            for ( i = l; i < d; i++) {
                out[l++] = ' ';
                out[l++] = '-';
            }
            out[l++] = ' ';
            out[l++] = ( defence ? hexdigit[tricklimit-lquirk[d]->ns] : hexdigit[lquirk[d]->ns] );
        }

        //  get the next distribution
        if ( lquirk[d]->d ) {
            d++;
            lquirk[d] = lquirk[d-1]->d;
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
                        printf( "%9.4f", 100.0 * result[d].tricks[i] );
                    printf( "%8.4f\n", tricklimit - result[d].result );
                }
                else {
                    for ( i = 0; i <= maxns - minns; i++ )
                        printf( "%9.4f", 100.0 * result[d].tricks[i] );
                    printf( "%8.4f\n", result[d].result );
                }
            }

            //  determine the best result
            if ( best_result.result < result[d].result )
                best_result.result = result[d].result;
            if ( defence )
                for ( i = 0; i < maxns - minns; i++ )
                    result[d].tricks[i+1] += result[d].tricks[i];
            else
                for ( i = maxns - minns; i > 0; i-- )
                    result[d].tricks[i-1] += result[d].tricks[i];
            if ( defence ) {
                for ( i = 0; i <= maxns - minns; i++ ) {
                    if ( best_result.tricks[i] > ( result[d].tricks[i] + EPS ) || (
                         best_result.tricks[i] > ( result[d].tricks[i] - EPS ) &&
                         best_result.res[i] < ( result[d].result + EPS ) ) ) {
                        best_result.tricks[i] = result[d].tricks[i];
                        best_result.res[i] = result[d].result;
                    }
                }
            }
            else {
                for ( i = 0; i <= maxns - minns; i++ ) {
                    if ( best_result.tricks[i] < ( result[d].tricks[i] - EPS ) || (
                         best_result.tricks[i] < ( result[d].tricks[i] + EPS ) &&
                         best_result.res[i] < ( result[d].result + EPS ) ) ) {
                        best_result.tricks[i] = result[d].tricks[i];
                        best_result.res[i] = result[d].result;
                    }
                }
            }

            //  store the score
            lquirk[d]->sc = &score[++ix];
            score[ix].ix = ix - 1;
            score[ix].result = result[d].result;
            for ( i = 0; i <= maxns - minns; i++ )
                score[ix].tricks[i] = result[d].tricks[i];
            score[ix].mp = NIL;

            //  get the next distribution
            while ( d && ! lquirk[d]->r )
                d--;
            l = 2 * d;
            out[l] = '\0';
            lquirk[d] = lquirk[d]->r;
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
