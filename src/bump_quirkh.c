//  name:
//      bump_quirk
//
//  purpose:
//      bump_quirk allocates space for QUIRKH entries
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

    if ( nextquirkh->h == NIL ) {

        memory += 8192 * sizeof( struct QUIRKH );
        quirk_memory += 8192 * sizeof( struct QUIRKH );
        nextquirkh->h = malloc( 8192 * sizeof( struct QUIRKH ) );
        if ( quirk_memory > memorylimit || nextquirkh->h == NIL ) {
            printf( "\nERROR: No more memory available: %6.1f MB after %3.1f seconds\n" , (float) memory / 1048576, TICK * clock() );
            exit( 1 );
        }

        memory += 8192 * boardcount * sizeof( struct TRICKS );
        quirk_memory += 8192 * boardcount * sizeof( struct TRICKS );
        trck = malloc( 8192 * boardcount * sizeof( struct TRICKS ) );
        if ( quirk_memory > memorylimit || trck == NIL ) {
            printf( "\nERROR: No more memory available: %6.1f MB after %3.1f seconds\n" , (float) memory / 1048576, TICK * clock() );
            exit( 1 );
        }

        wquirkh = nextquirkh->h;
        wquirkh->t = trck;
        for ( z = 1; z < 8192; z++ ) {
            wquirkh->h = wquirkh + 1;
            wquirkh = wquirkh->h;
            wquirkh->t = trck + z * boardcount;
        }
        wquirkh->h = NIL;
    }

    wquirkh = nextquirkh;
    nextquirkh = nextquirkh->h;
    wquirkh->d = NIL;
    wquirkh->sc = NIL;

    qhcount++;
    qhcountmax = MAX( qhcountmax, qhcount );
