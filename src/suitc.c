//  name:
//      suitc
//
//  purpose:
//      suitc is the main program for the analysis of suit combinations
//
//  to produce a list of options type:
//      SuitC {?|-?|-h}
//
//  Copyright 2024, Gerrit A. van der Velde, Leiden

#include "header.h"
#include "global.h"

int main( int argc, char **argv )
{
    int           i, d, p;

    //  set defaults and initial values
    buildtree = 0;
    cutoff = SUITCAPP ? CUTOFF : 0;
    defence = NO;
    eastfixed = RENONCE;
    eastvac = NCARDS;
    ewwinners = NO;
    foldmode = 1;    //  1=UP, 2=UPSTRIDE, 3=DOWN, 4=DOWNSTRIDE
    foldselect = 0x1 | 0x4;
    foldskip = 0;
    leader = noplayer;
    matchpoint = NO;
    matchpointtime = NO;
    maxnodes = 16000;
    ncardsmin[east] = 0;
    ncardsmin[west] = 0;
    for ( p = 0; p < 4; p++ )
        nfree[0][p] = NCARDS;
    for ( p = 0; p < 4; p++ )
        nruff[0][p] = 0;
    opening_lead = 0x7fff;
    for ( i = 0; i <= NCARDS; i++ )
        pointscore[i] = 0;
    printallmatchpoint = NO;
    printallresults = NO;
    printboards = NO;
    printfulltree = NO;
    printhighcard = NO;
    printoption = 2;
    printscard = SUITCAPP ? YES : NO;
    printsummary = NO;
    printtrunctree = NO;
    nrules = 0;
    reorder = SUITCAPP ? NO : YES;
    resultsonly = NO;
    for ( d = 0; d < NPLIES+1; d++ )
        quirkdl[d].ic = boardcount;
    trace = 0x0;
    tricklimit = NCARDS;
    usesuitentries = NO;
    westfixed = RENONCE;
    westvac = NCARDS;
    zermelo = 1;

    //  process the command line
    process_args( argc, argv );

    //  initialize the INDEX list
    icount = 0;
    icountmax = 0;
    nextindex = malloc( NINDICES * sizeof( struct INDEX ) );
    if ( ! nextindex ) {
        printf( "The memory assignment for the INDEX entries failed\n" );
        exit( 1 );
    }

    //  initialize the NODE tree
    ncount = 1;
    ncountmax = 1;
    wnode[0] = malloc( NNODES * sizeof( struct NODE ) );
    if ( ! wnode[0] ) {
        printf( "The memory assignment for the NODE entries failed\n" );
        exit( 1 );
    }
    nextnode = wnode[0];
    nextnode++;

    //  create the boards
    create_boards();

    //  build the tree
    create_tricks( 1 );

    //  print the tree
    if ( printtrunctree ) {
        printf( "\n\nThe truncated NODE tree\n" );
        printf(   "=======================\n" );
        print_tree( 1, NPLIES, wnode[0]->d, YES, YES );
        printf( "\n\n\n" );
        fflush( stdout );
    }

    times[0] = TICK * clock();

    //  initialise the QUIRKD list
    qdcount = 0;
    qdcountmax = 0;
    nextquirkd = malloc( 2 * NQUIRKS * sizeof( struct QUIRKD ) );
    if ( ! nextquirkd ) {
        printf( "The memory assignment for the QUIRKD entries failed\n" );
        exit( 1 );
    }

    //  initialise the QUIRKH and TRICKS lists
    qhcount = 0;
    qhcountmax = 0;
    nextquirkh = malloc( NQUIRKS * sizeof( struct QUIRKH ) );
    if ( ! nextquirkh ) {
        printf( "The memory assignment for the QUIRKH entries failed\n" );
        exit( 1 );
    }

    //  build the quirk tree
    convert_tree_to_quirk();

    //  determine best_result, the minimum and maximum number of tricks and the number of plays
    nplays = evaluate_best_results();

    if ( ! SUITCAPP ) {
        if ( ewwinners ) {
            printf( "\tmin number of losers: %d\n", tricklimit - maxns );
            printf( "\tmax number of losers: %d\n", tricklimit - minns );
        }
        else {
            printf( "\tmin number of tricks: %d\n", minns );
            printf( "\tmax number of tricks: %d\n", maxns );
        }
        printf( "\tnumber of plays     : %d\n", nplays );
    }

    //  restrict the number of plays for the MP or IMP analysis
    if ( matchpoint && nplays > maxnodes )
        limit_matchpoint_nodes();

    if ( ! SUITCAPP )
        printf( "\n\tCopyright 2026, Gerrit A. van der Velde, Leiden\n" );

    printf( "\n\n\nThe optimal results\n" );
    printf(   "===================\n\n" );
    if ( ewwinners ) {
        for ( i = maxns-1; i >= minns; i-- )
            printf( "\t%d losers: %8.4f\n", tricklimit - i, 100.0 * best_result.tricks[i] );
        printf( "\tResult  : %8.4f\n", tricklimit - best_result.result + EPS );
    }
    else {
        for ( i = maxns; i > minns; i-- )
            printf( "\t%d tricks: %8.4f\n", i, 100.0 * best_result.tricks[i] );
        printf( "\tResult  : %8.4f\n", best_result.result );
    }

    //  print a summary of the results
    if ( printsummary )
        print_summary_results();
    if ( printboards )
        print_boards();

    times[1] = TICK * clock();

    //  evaluate the MP or IMP plays
    if ( matchpoint ) {
        evaluate_mp_scores();
        evaluate_mp_results();
    }

    times[2] = TICK * clock();

    if ( ! resultsonly ) {

        //  rebuild the full game tree
        icount = boardcount;
        icountmax = boardcount;
        ncount = 1;
        ncountmax = 1;
        nextnode = wnode[0] + 1;
        buildtree = 1;
        create_tricks( 1 );

        //  print the tree
        if ( printfulltree ) {
            printf( "\n\n\nThe full NODE tree\n" );
            printf(   "==================\n" );
            print_tree( 1, NPLIES, wnode[0]->d, YES, YES );
            fflush( stdout );
        }
    }

    //  print the results
    buildtree = 2;
    print_all_results();
    times[3] = TICK * clock();
    print_stat( trace );
}
