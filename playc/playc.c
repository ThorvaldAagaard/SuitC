#include "header.h"

int main()
{
    int   i, n, cnt;
    COLOR c;

    PRINTFULL = YES;

    scanf( "%d", &n );
    {
	for ( i = 0; i < 8192; i++ ) {
	    c = (COLOR) i;
	    cnt = 0;
	    while ( c ) {
		cnt += 1;
		c ^= low_c( c );
	    }
	    if ( cnt == n ) {
	        hands[west] = (COLOR) i;
	        hands[east] = (COLOR) i;
	        hands[north] = (COLOR) (8192-1) ^ hands[west];
	        hands[south] = RENONCE;
	        create_all_boards();
	        print_boards();
	    }
	}
    }
}
