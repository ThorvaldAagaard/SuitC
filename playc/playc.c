#include "header.h"

int main()
{
    int   i;

    for ( i = 0; i < 8192; i++ ) {
        hands[west] = (COLOR) i;
        hands[east] = (COLOR) i;
        hands[north] = (COLOR) (8192-1) ^ hands[west];
        hands[south] = RENONCE;
        create_all_boards();
        print_boards();
    }
}
