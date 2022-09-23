//  name:
//      suitc
//
//  purpose:
//      suitc is the main program for the analysis of suit combinations
//
//  to produce a list of options type:
//      SuitC {?|-?|-h}
//
//  Copyright 2020, Gerrit A. van der Velde, Leiden

#include "header.h"

int main( int argc, char **argv )
{
    int           i, d, p;
    double        times[4];

    //  set defaults and initial values
    buildtree = 0;
    compacttree = YES;
    cutoff = SUITCAPP ? CUTOFF : 0;
    defence = NO;
    eastfixed = RENONCE;
    eastvac = NCARDS;
    keepewequal = YES;
    keepewweak = YES;
    keepnsequal = YES;
    keepnsweak = YES;
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
    printallmatchpoint = NO;
    printallresults = NO;
    printboards = NO;
    printfulltree = NO;
    printoption = 2;
    printsummary = NO;
    printtrunctree = NO;
    mergeselect = 0x1 | 0x4;
    nrules = 0;
    reorder = YES;
    resultsonly = NO;
    sortplays = NO;
    for ( d = 0; d <= NPLIES; d++ ) {
        st[d].ld = ( d & 3 ) == 1;
        st[d].t = ( d + 3 ) / 4;
    }
    throwin = noplayer;
    trace = 0x0;
    tricklimit = NCARDS;
    for ( d = 0; d < NPLIES+2; d++ )
        quirkd[d].ic = boardcount;
    w[0] = noplayer;
    westfixed = RENONCE;
    westvac = NCARDS;
    zermelo = 1;

    //  process the command line
    process_args( argc, argv );

    //  initialize the INDEX list
    icount = 0;
    icountmax = 0;
    nextindex = malloc( 12000000 * sizeof( struct INDEX ) );
    if ( ! nextindex ) {
        printf( "The memory assignment for the INDEX entries failed\n" );
        exit( 1 );
    }

    //  initialize the NODE tree
    ncount = 1;
    ncountmax = 1;
    wnode[0] = malloc( 8000000 * sizeof( struct NODE ) );
    if ( ! wnode[0] ) {
        printf( "The memory assignment for the NODE entries failed\n" );
        exit( 1 );
    }
    nextnode = wnode[0];
    nextnode++;

    //  create the boards
    create_boards();

    //  build the tree
    ns[0] = defence ? tricklimit : 0;
    create_tricks( 1 );

    //  print the tree
    if ( printtrunctree ) {
        printf( "\n\nThe truncated NODE tree\n" );
        printf(   "=======================\n" );
        print_tree( 0, wnode[0], YES, YES );
        printf( "\n\n\n" );
        fflush( stdout );
    }

    times[0] = TICK * clock();

    //  initialise the QUIRKD list
    qdcount = 0;
    qdcountmax = 0;
    nextquirkd = malloc( 2 * NPLAYS * sizeof( struct QUIRKD ) );
    if ( ! nextquirkd ) {
        printf( "The memory assignment for the QUIRKD entries failed\n" );
        exit( 1 );
    }

    //  initialise the QUIRKH and TRICKS lists
    qhcount = 0;
    qhcountmax = 0;
    nextquirkh = malloc( NPLAYS * sizeof( struct QUIRKH ) );
    if ( ! nextquirkh ) {
        printf( "The memory assignment for the QUIRKH entries failed\n" );
        exit( 1 );
    }
    tricks = malloc( (long) boardcount * NPLAYS * sizeof( struct TRICKS ) );
    if ( ! tricks ) {
        printf( "The memory assignment for the TRICKS entries failed\n" );
        exit( 1 );
    }

    //  build the quirk tree
    if ( sortplays ) {
        count_plays();
        sort_plays();
    }
    convert_tree_to_quirk();

    //  determine best_result, the minimum and maximum number of tricks and the number of plays
    nplays = evaluate_best_results();

    if ( ! SUITCAPP ) {
        if ( defence ) {
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
        printf( "\n\tCopyright 2022, Gerrit A. van der Velde, Leiden\n" );

    if ( sortplays )
        print_play_count();

    printf( "\n\n\nThe optimal results\n" );
    printf(   "===================\n\n" );
    if ( defence ) {
        for ( i = maxns - 1; i >= minns; i-- )
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
            print_tree( 0, wnode[0], YES, YES );
            fflush( stdout );
        }
    }

    //  print the results
    buildtree = 2;
    print_all_results();

    times[3] = TICK * clock();

    if ( trace & 0x1 ) {
        printf( "\n\n\n\tCounter        Maxvalue    Memory\n\t-------        --------    ------\n" );
        printf( "\tindexcount%13d%10.1f\n", icountmax, icountmax * sizeof( struct INDEX ) / 1048576. );
        printf( "\tnodecount%14d%10.1f\n", ncountmax, ncountmax * sizeof( struct NODE ) / 1048576. );
        printf( "\tquirkdcount%12d%10.1f\n", qdcountmax, qdcountmax * sizeof( struct QUIRKD ) / 1048576. );
        printf( "\tquirkhcount%12d%10.1f\n", qhcountmax, qhcountmax * sizeof( struct QUIRKH ) / 1048576. );
        printf( "\ttrickscount%12d%10.1f\n", qhcountmax, (long) qhcountmax * boardcount * sizeof( struct TRICKS ) / 1048576. );
        printf( "\n\tEvent                              Time\n" );
        printf( "\t--------------------------         ----\n" );
        printf( "\tThe tree is created        %12.3f\n", times[0] );
        printf( "\tThe quirk tree is build    %12.3f\n", times[1] );
        if ( matchpointtime == IMP )
            printf( "\tThe IMP plays are evaluated%12.3f\n", times[2] );
        if ( matchpointtime == MP )
            printf( "\tThe MP plays are evaluated %12.3f\n", times[2] );
        printf( "\tThe program is finished    %12.3f\n", times[3] );
    }

    printf( "\n\n\n\n" );
    exit( 0 );
}
