//  name:
//      add_quirkh
//
//  purpose:
//      add_quirkh adds a new entry to the QUIRKH tree
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

next_branch:
    if ( m == nquirkd[n]->ih + 1 && ! nquirkd[n]->d ) {
        nquirkd[n]->ih = m;
        nquirkdl[n]->h->t[m].i = i;
        nquirkdl[n]->h->t[m].ns = ns;
    }

    else if ( ns == nquirkdl[n]->h->t[m].ns && m == nquirkd[n]->ih && nquirkd[n]->d ) {
        n++;
        nquirkdl[n] = nquirkdl[n-1];
        nquirkd[n] = nquirkd[n-1]->d;
    }

    else if ( ns == nquirkdl[n]->h->t[m].ns ) ;

    else if ( ns > nquirkdl[n]->h->t[m].ns && m == nquirkd[n]->il && nquirkd[n] == nquirkdl[n]->h->d ) {

        nquirkhn = nextquirkh;
        #include "bump_quirkh.c"
        nquirkhn->d = nextquirkd;
        #include "bump_quirkd.c"
        nquirkhn->d->h = nquirkdl[n]->h;
        nquirkdl[n]->h = nquirkhn;

        nquirkd[n] = nquirkdl[n]->h->d;
        nquirkd[n]->il = m;
        nquirkd[n]->ih = m;
        nquirkdl[n]->h->t[m].i = i;
        nquirkdl[n]->h->t[m].ns = ns;
    }

    else if ( ns > nquirkdl[n]->h->t[m].ns && m == nquirkd[n]->il && nquirkd[n] == nquirkd[n-1]->d ) {

        nquirkdn = nextquirkd;
        #include "bump_quirkd.c"
        nquirkdn->h = nextquirkh;
        #include "bump_quirkh.c"
        nquirkd[n-1]->d = nquirkdn;
        nquirkdn->h->d = nquirkd[n];

        nquirkdn->il = m;
        nquirkdn->ih = m;
        for ( h = m; h <= ih; h++ )
             nquirkdn->h->t[h] = nquirkdl[n]->h->t[h];
        nquirkdl[n]->h->t[m].i = i;
        nquirkdl[n]->h->t[m].ns = ns;

        nquirkd[n] = nquirkdn;
    }

    else if ( ns > nquirkdl[n]->h->t[m].ns && m > nquirkd[n]->il ) {

        nquirkdn = nextquirkd;
        #include "bump_quirkd.c"
        nquirkdn->h = nextquirkh;
        #include "bump_quirkh.c"
        nquirkdn->h->d = nextquirkd;
        #include "bump_quirkd.c"
        nquirkdn->h->d->d = nquirkd[n]->d;
        nquirkd[n]->d = nquirkdn;

        nquirkdn->il = m;
        nquirkdn->ih = m;
        nquirkdn->h->d->il = m;
        nquirkdn->h->d->ih = nquirkd[n]->ih;
        nquirkd[n]->ih = m-1;
        for ( h = m; h <= ih; h++ )
             nquirkdn->h->t[h] = nquirkdl[n]->h->t[h];
        nquirkdl[n]->h->t[m].i = i;
        nquirkdl[n]->h->t[m].ns = ns;

        n++;
        nquirkd[n] = nquirkdn;
        nquirkdl[n] = nquirkdl[n-1];
    }

    else if ( ns < nquirkdl[n]->h->t[m].ns && m > nquirkd[n]->il ) {

        nquirkdn = nextquirkd;
        #include "bump_quirkd.c"
        nquirkdn->h = nextquirkh;
        #include "bump_quirkh.c"
        nquirkdn->h->d = nextquirkd;
        #include "bump_quirkd.c"
        nquirkdn->d = nquirkd[n]->d;
        nquirkd[n]->d = nquirkdn;

        nquirkdn->il = m;
        nquirkdn->ih = nquirkd[n]->ih;
        nquirkdn->h->d->il = m;
        nquirkdn->h->d->ih = m;
        nquirkd[n]->ih = m-1;
        nquirkdn->h->t[m].i = i;
        nquirkdn->h->t[m].ns = ns;

        n++;
        nquirkdl[n] = nquirkdn;
        nquirkd[n] = nquirkdn->h->d;
    }

    else if (  ns < nquirkdl[n]->h->t[m].ns && m == nquirkd[n]->il && nquirkd[n]->h ) {
        nquirkdl[n] = nquirkd[n];
        nquirkd[n] = nquirkdl[n]->h->d;
        goto next_branch;
    }

    else if ( ns < nquirkdl[n]->h->t[m].ns && m == nquirkd[n]->il && nquirkd[n]->h == NIL ) {

        nquirkhn = nextquirkh;
        #include "bump_quirkh.c"
        nquirkhn->d = nextquirkd;
        #include "bump_quirkd.c"
        nquirkd[n]->h = nquirkhn;

        nquirkhn->d->il = m;
        nquirkhn->d->ih = m;
        nquirkhn->t[m].i = i;
        nquirkhn->t[m].ns = ns;

        nquirkdl[n] = nquirkd[n];
        nquirkd[n] = nquirkhn->d;
    }
