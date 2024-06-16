//  name:
//      print_boards
//
//  purpose:
//      print_boards prints the information about the boards
//
//  parameters:
//      none
//
//  Copyright 2024, Gerrit A. van der Velde, Leiden

#include "header.h"

void print_boards( void )
{
    int          i, j, k, p;
    char         out[NCARDS+1];
    static char  pad[] = "                    ";
    static char  *symbols[] = { "-123456789TJQKA", "-123456789TBVHA", "-123456789tjqka", "-123456789tbvha" };
    struct INDEX *lindex;

    pad[MAX(6,ncards[west])-1] = '\0';
    printf( "\n\n\n%s%s%s%s%s\n%s%s%s%s%s\n",
            "       Probabilities    West", pad, "East", pad, "tricks",
            "     ===============    ====", pad, "====", pad, "======" );
    pad[MAX(6,ncards[west])-1] = ' ';

    i = 0;
    lindex = wnode[0]->i;
    while ( lindex ) {

        printf( "%6d x %11.8f    ", dupl[i], 100. * prob[i] );
        for ( p = 0; p < 4; p += 2 ) {
            k = 0;
            out[0] = '-';
            out[1] = '\0';
            for ( j = NCARDS+1; j >= 0; j-- ) {
                if ( addcard(j) & lindex->c[p] ) {
                    out[k] = symbols[printoption&0x1][j];
                    out[++k] = '\0';
                }
            }
            k = out[0] == '-' ? k+1 : k;
            pad[ncards[p]-p-k+3] = '\0';
            printf( "%s%s", out, pad );
            pad[ncards[p]-p-k+3] = ' ';
        }
        printf( "%4d -%2d\n", nslimits[0][i], nslimits[1][i] );

        i++;
        lindex = lindex->r;
    }

    fflush( stdout );
}
