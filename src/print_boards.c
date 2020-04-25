//  name:
//      print_boards
//
//  purpose:
//      print_boards prints the information about the suit combination
//
//  parameters:
//      none
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

#include "header.h"

void print_boards( void )
{
    int          i, j, k, n, p, q;
    char         out[14];
    static char  *symbols[] = { "-123456789TJQKA", "-123456789TBVHA", "-123456789tjqka", "-123456789tbvha" };
    static char  seat[4][6] = { "west", "north", "east", "south" };
    struct INDEX *lindex;

    //  print the 4 hands
    for ( q = 0; q < 4; q++ ) {
        p = ( q == 0 ? north : ( q == 1 ? south : ( q == 2 ? west : east ) ) );
        k = 0;
        out[0] = '-';
        out[1] = '\0';
        for ( i = NCARDS+1; i >= 0; i-- ) {
            if ( addcard(i) & brd[0][p] ) {
                out[k] = symbols[printoption&0x1][i];
                out[++k] = '\0';
            }
        }
        printf( "\t%-5s: %s\n", seat[p], out );
    }
    printf( "\n" );

    //  print additional information
    if ( cutoff )
        printf( "\tcutoff              : %.0fe6\n", 1.e-6 * cutoff );
    if ( tricklimit < ntricks )
        printf( "\tterminate play after: %d tricks\n", tricklimit );
    if ( w[0] != noplayer ) {
        printf( "\tthe lead is in      : %s\n", seat[w[0]] );
        if ( opening_lead != 0x7fff )
            for ( i = 0; i <= NCARDS+1; i++ )
                if ( opening_lead & addcard(i) )
                    printf( "\tthe lead is the     : %c\n", symbols[printoption&0x1][i] );
    }
    for ( p = 0; p < 4; p++ )
        if ( nfree[0][p] < ncards[p] )
            printf( "\t#free entries %-5s : %d\n", seat[p], nfree[0][p] );
    for ( p = 0; p < 4; p++ )
        if ( nruff[0][p] )
            printf( "\tnumb. of ruffs %-5s: %d\n", seat[p], nruff[0][p] );
    if ( impscore ) {
        for ( n = 0; n <= ntricks; n++ )
            if ( pointscore[n] )
                printf( "\t#points for %d tricks: %d\n", n, pointscore[n] );
    }
    if ( eastfixed ) {
        k = 0;
        out[0] = '-';
        out[1] = '\0';
        for ( i = NCARDS+1; i >= 0; i-- ) {
            if ( addcard(i) & eastfixed ) {
                out[k] = symbols[printoption&0x1][i];
                out[++k] = '\0';
            }
        }
        printf( "\tfixed east cards    : %s\n", out );
    }
    if ( westfixed ) {
        k = 0;
        out[0] = '-';
        out[1] = '\0';
        for ( i = NCARDS+1; i >= 0; i-- ) {
            if ( addcard(i) & westfixed ) {
                out[k] = symbols[printoption&0x1][i];
                out[++k] = '\0';
            }
        }
        printf( "\tfixed west cards    : %s\n", out );
    }
    if ( ncardsmin[west] != 0 )
        printf( "\tminimum length west : %d\n", ncardsmin[west] );
    if ( westvac != NCARDS )
        printf( "\t#vacant places west : %d\n", westvac );
    if ( ncardsmin[east] != 0 )
        printf( "\tminimum length east : %d\n", ncardsmin[east] );
    if ( eastvac != NCARDS )
        printf( "\t#vacant places east : %d\n", eastvac );

    //  print the full set of boards
    if ( printboards ) {

        i = 0;
        lindex = wnode[0]->i;
        printf( "\n\n%s\n%s\n",
                "   Probabilities    West         North        East         South",
                "   =============    ====         =====        ====         =====" );

        while ( lindex ) {

            printf( "%5d x %7.6f    ", dupl[i], prob[i] );
            for ( p = 0; p < 4; p++ ) {
                k = 0;
                out[0] = '-';
                out[1] = '\0';
                for ( j = NCARDS+1; j >= 0; j-- ) {
                    if ( addcard(j) & lindex->c[p] ) {
                        out[k] = symbols[printoption&0x1][j];
                        out[++k] = '\0';
                    }
                }
                printf( "%-13s", out );
            }
            printf( "\n" );

            i++;
            lindex = lindex->r;
        }
        printf( "\n\n" );
    }
    fflush( stdout );
}
