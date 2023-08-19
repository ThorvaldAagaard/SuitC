//  name:
//      process_args
//
//  purpose:
//      process_args processes the command line
//
//  to produce a list of options:
//      dds {?|-h|-?}
//
//  parameters:
//      none
//
//  Copyright 2023, Gerrit A. van der Velde, Leiden

void process_args( int argc, char **argv )
{
    char          c;
    int           i, k, match, n, p = west, printoptions = NO, s;
    static char   *symbols[] = { "-123456789tjqka", "-123456789tbvha", "-123456789TJQKA", "-123456789TBVHA" };

    for ( i = 1; i < argc; i++ ) {

        //  process the options
        if ( argv[i][0] == '?' )
            printoptions = YES;
        else if ( argv[i][0] == '-' ) {
            c = argv[i][1];
            if ( c == 'h' || c == '?' )
                printoptions = YES;
            else if ( c == 'L' ) {
                c = argv[i][2];
                ld = c == 'w' || c == 'W'                        ? west
                   : c == 'n' || c == 'N'                        ? north
                   : c == 'e' || c == 'E' || c =='o' || c == 'O' ? east
                   : c == 's' || c == 'S' || c =='z' || c == 'Z' ? south
                   :                                               unknown;
                if ( ld == unknown ) {
                    printf( "Incorrect leader: %s; program stops\n", argv[i] );
                    exit( 99 );
                }
                lnode[0]->w_p = ld;
            }
            else if ( c == 'T' ) {
                c = argv[i][2];
                trump = c == 's' || c == 'S'                         ? spades
                      : c == 'h' || c == 'H'                         ? hearts
                      : c == 'd' || c == 'D' || c == 'r' || c == 'R' ? diamonds
                      : c == 'c' || c == 'C' || c == 'k' || c == 'K' ? clubs
                      : c == 'n' || c == 'N'                         ? notrump
                      :                                                discard;
                if ( trump == discard ) {
                    printf( "Incorrect trump suit: %s; program stops\n", argv[i] );
                    exit( 99 );
                }
            }
            else {
                printf( "Incorrect option: %s; program stops\n", argv[i] );
                exit( 99 );
            }
        }

        //  enter the hands in the order: west, north, east, south
        else {
            k = 0;
            s = spades;
            brd[blist[0].n][0][s][p] = RENONCE;
            while ( argv[i][k] ) {
                c = argv[i][k];
                if ( c == '.' ) {
                    s++;
                    brd[blist[0].n][0][s][p] = RENONCE;
                }
                else {
                    match = NO;
                    for ( n = two; n <= ace; n++ ) {
                        if ( ( c == symbols[0][n] ) || ( c == symbols[1][n] ) || ( c == symbols[2][n] ) || ( c == symbols[3][n] ) ) {
                            match = YES;
                            brd[blist[0].n][0][s][p] |= set(n);
                        }
                    }
                    if ( ! match ) {
                        printf( "Incorrect symbol: %c; program stops\n", c );
                        exit( 99 );
                    }
                }
                k++;
            }
            if ( p == south ) {
                blist[0].b[blist[0].n] = blist[0].n;
                blist[0].n++;
                p = west;
            }
            else
                p++;
        }
    }

    if ( printoptions ) {
        printf( "\n" );
        printf( "calling sequence:\n" );
        printf( "\n" );
        printf( "dds [{?|-h|-?}] [-L{w|n|e|s}] [-T{s|h|d|c|n}] <west> <north> <east> <south> <west> <north> <east> <south> ...\n" );
        printf( "\noptions:\n" );
        printf( "    ?|-h|-?         print the options of dds\n" );
        printf( "    -L{w|n|e|s}     the opening leader at trick one\n" );
        printf( "    -T{s|h|d|c|n}   the trump suit\n" );
        printf( "    <west>          the west hand\n" );
        printf( "    <north>         the north hand\n" );
        printf( "    <east>          the east hand\n" );
        printf( "    <south>         the south hand\n" );
        printf( "                    each hand is described as <spades>.<hearts>.<diamonds>.<clubs>\n" );
        printf( "                    both English and Dutch card symbols are accepted in upper and lower case\n" );
        printf( "\n" );

        exit( 0 );
    }
}
