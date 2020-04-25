//  name:
//      bump_quirk
//
//  purpose:
//      bump_quirk allocates space for QUIRKD entries
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

    if ( nextquirkd->d == NIL ) {

        memory += 8192 * sizeof( struct QUIRKD );
        quirk_memory += 8192 * sizeof( struct QUIRKD );
        nextquirkd->d = malloc( 8192 * sizeof( struct QUIRKD ) );
        if ( quirk_memory > memorylimit || nextquirkd->d == NIL ) {
            printf( "\nERROR: No more memory available: %6.1f MB after %3.1f seconds\n" , (float) memory / 1048576, TICK * clock() );
            exit( 1 );
        }

        wquirkd = nextquirkd->d;
        for ( z = 1; z < 8192; z++ ) {
            wquirkd->d = wquirkd + 1;
            wquirkd = wquirkd->d;
        }
        wquirkd->d = NIL;
    }

    wquirkd = nextquirkd;
    nextquirkd = nextquirkd->d;
    wquirkd->d = NIL;
    wquirkd->h = NIL;
    wquirkd->ic = NBOARDS;

    qdcount++;
    qdcountmax = MAX( qdcountmax, qdcount );
