//  name:
//      print_scard
//
//  purpose:
//      print_scard prints a SCARD node
//
//  parameters:
//      root            in      the SCARD node to be printed
//      id              in      an identification string

//  #include "header.h"

void print_scard( struct SCARD *root, char *id )
{
    int           i, k, n;
    char          str[14];
    static char   seats[] = "wnes";
    static char   symbols[] = "-123456789tjqka=";
    static char   *suit[] = { "spades", "hearts", "diamonds", "clubs", "notrump", "discard" };
    struct SEQC   *sq;

    if ( ! root )
        return;

    printf( "print_scard   brd={%04x,%04x,%04x,%04x}", root->brd[0] >> 2, root->brd[1] >> 2, root->brd[2] >> 2, root->brd[3] >> 2 );

    sq = root->sq;
    for ( n = 0; n < root->n; n++ ) {

        k = 0;
        for ( i = ace+1; i >= 0; i-- ) {
            if ( set(i) & sq[n].c )
                str[k++] = symbols[i];
        }

        str[k] = '\0';
        printf( "    %c:%x:%s", seats[sq[n].p], sq[n].n, str );
    }

    printf( "    %s\n", id );
}
