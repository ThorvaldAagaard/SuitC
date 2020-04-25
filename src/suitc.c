//  name:
//      suitc
//
//  purpose:
//      suitc is the main program for the analysis of suit combinations
//
//  to produce a list of options type:
//      ExpertPlay -h
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

int main( int argc, char **argv )
{
    int           i, d, p, z;
    struct INDEX  zindex;
    struct NODE   *lnode[NPLIES+1], znode;
    struct QUIRK  zquirk;
    struct QUIRKD *wquirkd, zquirkd;
    struct QUIRKH *wquirkh, zquirkh;
    struct TRICKS *trck;
    struct { double  m, t; } times[5];

    //  set defaults and initial values
    cutoff = 0.e0;
    defence = NO;
    deleteterm = NO;
    eastfixed = RENONCE;
    eastvac = NCARDS;
    icount = -1;
    icountmax = 0;
    impscore = NO;
    keepewequal = YES;
    keepfulltree = NO;
    keepnsequal = YES;
    keepewweak = YES;
    keepnsweak = YES;
    leader = noplayer;
    limittricks = NO;
    matchpoint = NO;
    maxnodes = 16000;
    memory = 0LL * 1048576LL;
    memorylimit = MEMORY * 1048576LL;
    ncardsmin[east] = 0;
    ncardsmin[west] = 0;
    ncount = -1;
    ncountmax = 0;
    for ( p = 0; p < 4; p++ )
        nfree[0][p] = NCARDS;
    for ( p = 0; p < 4; p++ )
        nruff[0][p] = 0;
    opening_lead = 0x7fff;
    for ( i = 0; i <= NCARDS; i++ )
        pointscore[i] = 0;
    printallresults = NO;
    printboards = NO;
    printgametree = NO;
    printoption = 0;
    printsummary = NO;
    mergeselect = 0x1 | 0x2;
    qcount = -1;
    qcountmax = 0;
    qdcount = -1;
    qdcountmax = 0;
    qhcount = -1;
    qhcountmax = 0;
    quirk_memory = 0LL * 1048576LL;
    qhd = 2;
    reorder = NO;
    resultsonly = NO;
    for ( d = 0; d <= NPLIES; d++ ) {
        st[d].ld = ( d & 3 ) == 1;
        st[d].t = ( d + 3 ) / 4;
    }
    throwin = NO;
    trace = 0x0;
    tricklimit = NCARDS;
    for ( d = 0; d < NPLIES+2; d++ )
        tquirkd[d].ic = NBOARDS;
    w[0] = noplayer;
    westfixed = RENONCE;
    westvac = NCARDS;
    zermelo = 1;

    //  process the command line
    process_args( argc, argv );

    //  initialize the index list
    nextindex = &zindex;
    nextindex->r = NIL;
    bump_index();

    //  initialize the node tree
    wnode[0] = &znode;
    nextnode = &znode;
    nextnode->r = NIL;
    bump_node();

    //  create the boards
    create_boards();

    //  build the tree
    ns[0] = ( defence ? tricklimit : 0 );
    create_tricks( 1 );

    //  print the tree
    if ( deleteterm ) {
        printf( "\n\nThe truncated NODE tree\n" );
        printf(   "=======================\n" );
        print_tree( 1, wnode[0]->d, YES, YES );
        printf( "\n\n\n" );
        fflush( stdout );
    }

    times[0].t = TICK * clock();
    times[0].m = (float) memory / 1048576;

    //  initialise the QUIRK list
    nextquirk = &zquirk;
    nextquirk->r = NIL;
    bump_quirk();

    //  initialise the QUIRKD list
    nextquirkd = &zquirkd;
    nextquirkd->d = NIL;
    #include "bump_quirkd.c"

    //  initialise the QUIRKH list
    nextquirkh = &zquirkh;
    nextquirkh->h = NIL;
    #include "bump_quirkh.c"

    //  build the quirk tree
    convert_tree_to_quirk( 0 );

    //  reset the minimum and maximum number of tricks as the wrong values might
    //  have been obtained from the limiting process on the number of distributions
    if ( qhd == 1 )
        nplays = tquirk[0] ? count_quirk( tquirk[0]->d ) : 0;
    if ( qhd == 2 )
        count_quirkh( &tquirkd[0] );

    if ( defence ) {
        printf( "\tmin number of losers: %d\n", tricklimit - maxns );
        printf( "\tmax number of losers: %d\n", tricklimit - minns );
    }
    else {
        printf( "\tmin number of tricks: %d\n", minns );
        printf( "\tmax number of tricks: %d\n", maxns );
    }

    printf( "\tnumber of plays     : %d\n", nplays );
    printf( "\n\tCopyright 2019, Gerrit A. van der Velde, Leiden\n" );

    times[1].t = TICK * clock();
    times[1].m = (float) memory / 1048576;

    if ( ! resultsonly ) {

        //  rebuild the full game tree
        lnode[0] = wnode[0];
        lnode[1] = wnode[0]->d;
        while ( lnode[1] )
            lnode[1] = delete_tree( 1, lnode );
        keepfulltree = YES;
        create_tricks( 1 );

        //  print the tree
        if ( printgametree ) {
            printf( "\n\n\nThe full NODE tree\n" );
            printf(   "==================\n" );
            print_tree( 1, wnode[0]->d, YES, YES );
            fflush( stdout );
        }
    }

    //  determine best.result (and print all remaining plays)
    if ( qhd == 1 )
        evaluate_all_results_quirk();
    if ( qhd == 2 )
        evaluate_all_results_quirkh();

    printf( "\n\n\nThe optimal results\n" );
    printf(   "===================\n\n" );
    if ( defence ) {
        for ( i = maxns - minns - 1; i >= 0; i-- )
            printf( "\t%d losers: %8.4f\n", tricklimit - i - minns, 100.0 * best_result.tricks[i] );
        printf( "\tResult  : %8.4f\n", tricklimit - best_result.result + EPS );
    }
    else {
        for ( i = maxns - minns; i > 0; i-- )
            printf( "\t%d tricks: %8.4f\n", i + minns, 100.0 * best_result.tricks[i] );
        printf( "\tResult  : %8.4f\n", best_result.result );
    }

    times[2].t = TICK * clock();
    times[2].m = (float) memory / 1048576;

    //  evaluate the matchpoint or IMP plays
    mp_memory = memory;
    if ( impscore || matchpoint ) {
        if ( matchpoint && ( nplays > maxnodes ) )
            limit_matchpoint_nodes();
        if ( qhd == 1 )
            evaluate_mp_scores_quirk();
        if ( qhd == 2 )
            evaluate_mp_scores_quirkh();
        evaluate_mp_results();
    }

    times[3].t = TICK * clock();
    times[3].m = (double) mp_memory / 1048576;

    //  print the results
    if ( resultsonly ) {
        if ( qhd == 1 )
            print_results_only_quirk();
        if ( qhd == 2 )
            print_results_only_quirkh();
    }
    else {
        limittricks = YES;
        if ( qhd == 1 )
            print_all_results_quirk();
        if ( qhd == 2 )
            print_all_results_quirkh();
    }

    //  delete the NODE and QUIRK trees
    lnode[0] = wnode[0];
    lnode[1] = lnode[0]->d;
    while ( lnode[1] )
        delete_tree( 1, lnode );
    if ( qhd == 1 )
        delete_quirk( tquirk[0] );
    if ( qhd == 2 )
        delete_quirkh( 0 );

    times[4].t = TICK * clock();
    times[4].m = (float) memory / 1048576;

    if ( trace & 0x1 ) {
        printf( "\n\n\n\n\tCounter        Maxvalue\n\t-------        --------\n" );
        printf( "\tindexcount%13d\n", icountmax );
        printf( "\tnodecount%14d\n", ncountmax );
        if ( qhd == 1 )
            printf( "\tquirkcount%13d\n", qcountmax );
        if ( qhd == 2 )
            printf( "\tquirkdcount%12d\n", qdcountmax );
        if ( qhd == 2 )
            printf( "\tquirkhcount%12d\n", qhcountmax );
        printf( "\n\tEvent                              Time   Memory\n" );
        printf( "\t--------------------------         ----   ------\n" );
        printf( "\tThe tree is created        %12.3f%9.1f\n", times[0].t, times[0].m );
        printf( "\tThe quirk tree is build    %12.3f%9.1f\n", times[1].t, times[1].m );
        printf( "\tThe results are evaluated  %12.3f%9.1f\n", times[2].t, times[2].m );
        printf( "\tThe MP plays are evaluated %12.3f%9.1f\n", times[3].t, times[3].m );
        printf( "\tThe program is finished    %12.3f%9.1f\n", times[4].t, times[4].m );
    }

    printf( "\n\n\n\n" );
    exit( 0 );
}
