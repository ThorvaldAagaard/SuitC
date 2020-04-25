//  name:
//      check_quirkh
//
//  purpose:
//      check_quirkh compares the branches in a QUIRKH tree and deletes all inferior ones branches.
//      check_quirkh is called at the end of fold_quirkh and merge_quirkh.
//
//  parameters:
//      n               in      the depth in the QUIRKH tree
//      d               in      the depth in the NODE tree
//      nquirkd         in      the QUIRKH tree
//      nquirkdl        in      the left siblings in the QUIRKH tree
//
//  Copyright 2019, Gerrit A. van der Velde, Leiden

    u = n;
    v = n;
    ic = nquirkd[n]->il;
    s = nquirkd[n]->il;
    vquirkdl[v] = nquirkd[n];
    vquirkd[v] = vquirkdl[n]->h->d;

    while ( nquirkd[n]->h ) {

        //  compare both branches; exit if the v-branch beats the u-branch
        while ( YES ) {

            if ( vquirkd[v]->ic <= ic && nquirkd[u]->ic <= ic ||
                 vquirkdl[v]->h->t[s].ns > nquirkdl[u]->h->t[s].ns ||
                 ( s == vquirkd[v]->ih && ! vquirkd[v]->d ) )
                break;

            s++;

            if ( s > nquirkd[u]->ih && nquirkd[u]->d ) {
                u++;
                nquirkdl[u] = nquirkdl[u-1];
                nquirkd[u] = nquirkd[u-1]->d;
            }

            if ( s > vquirkd[v]->ih && vquirkd[v]->d ) {
                v++;
                vquirkdl[v] = vquirkdl[v-1];
                vquirkd[v] = vquirkd[v-1]->d;
            }
        }

        //  delete inferior branches
        if ( s == vquirkd[v]->ih && ! vquirkd[v]->d && vquirkdl[v]->h->t[s].ns <= nquirkdl[u]->h->t[s].ns ) {

            ih = vquirkd[v]->ih;

            s = vquirkd[v]->il;
            while ( nquirkd[u]->il > s )
                u--;

            if ( vquirkd[v] == vquirkdl[v]->h->d ) {

                qhcount--;
                vquirkdl[v]->h->h = nextquirkh;
                nextquirkh = vquirkdl[v]->h;

                vquirkdl[v]->h = vquirkd[v]->h;

                qdcount--;
                vquirkd[v]->d = nextquirkd;
                nextquirkd = vquirkd[v];

                if ( vquirkdl[v]->h ) {

                    vquirkd[v] = vquirkdl[v]->h->d;

                    if ( u > n && nquirkd[u]->il == vquirkd[v]->il ) {
                        nquirkdl[u] = nquirkdl[u-1];
                        nquirkd[u] = nquirkd[u-1]->d;
                    }

                    continue;
                }

                else if ( v > n && vquirkdl[v] == vquirkd[v-1]->d ) {

                    vquirkd[v-1]->d = vquirkdl[v]->d;
                    vquirkd[v-1]->ih = vquirkdl[v]->ih;

                    qdcount--;
                    vquirkdl[v]->d = nextquirkd;
                    nextquirkd = vquirkdl[v];

                    v--;
                }

                else if ( n && v == n && vquirkdl[v] == nquirkd[n-1]->d ) {

                    nquirkd[n-1]->d = nquirkd[n]->d;
                    nquirkd[n-1]->ih = nquirkd[n]->ih;

                    qdcount--;
                    nquirkd[n]->d = nextquirkd;
                    nextquirkd = nquirkd[n];

                    v--;

                    break;
                }

                else if ( v == n ) {

                    if ( nquirkd[n]->h ) {

                        nquirkdl[n] = nquirkd[n];
                        nquirkd[n] = nquirkdl[n]->h->d;

                        if ( nquirkd[n]->h ) {

                            vquirkdl[v] = nquirkd[n];
                            vquirkd[v] = vquirkdl[v]->h->d;

                            continue;
                        }

                        break;
                    }
                }

                else
                    vquirkd[v] = vquirkdl[v];
            }

            else if ( v > n && vquirkd[v] == vquirkd[v-1]->d ) {

                vquirkd[v-1]->d = vquirkd[v]->h->d;
                for ( h = vquirkd[v]->il; h <= ih; h++ )
                    vquirkdl[v]->h->t[h] = vquirkd[v]->h->t[h];

                qhcount--;
                vquirkd[v]->h->h = nextquirkh;
                nextquirkh = vquirkd[v]->h;

                qdcount--;
                vquirkd[v]->d = nextquirkd;
                nextquirkd = vquirkd[v];

                vquirkd[v] = vquirkd[v-1]->d;

                if ( u > n && nquirkd[u]->il == vquirkd[v]->il ) {
                    nquirkdl[u] = nquirkdl[u-1];
                    nquirkd[u] = nquirkd[u-1]->d;
                }

                if ( vquirkd[v]->h == NIL ) {

                    vquirkd[v-1]->d = vquirkd[v]->d;
                    vquirkd[v-1]->ih = vquirkd[v]->ih;

                    qdcount--;
                    vquirkd[v]->d = nextquirkd;
                    nextquirkd = vquirkd[v];

                    v--;
                }

                continue;
            }
        }

        //  locate the next branches to be compared
        while ( YES ) {

            if ( u == n && v == n ) {

                if ( vquirkd[v]->h ) {
                    s = vquirkd[v]->il;
                    vquirkdl[v] = vquirkd[v];
                    vquirkd[v] = vquirkdl[v]->h->d;
                    break;
                }

                else {

                    if ( ! nquirkd[u]->h )
                        break;

                    s = nquirkd[u]->il;
                    nquirkdl[u] = nquirkd[u];
                    nquirkd[u] = nquirkd[u]->h->d;
                    if ( ! nquirkd[u]->h )
                        break;

                    s = vquirkd[v]->il;
                    vquirkdl[v] = nquirkd[v];
                    vquirkd[v] = vquirkdl[v]->h->d;
                    break;
                }
            }

            else if ( vquirkd[v]->il == nquirkd[u]->il ) {

                if ( vquirkd[v]->h ) {
                    s = vquirkd[v]->il;
                    vquirkdl[v] = vquirkd[v];
                    vquirkd[v] = vquirkdl[v]->h->d;
                    break;
                }

                else if ( nquirkd[u]->h && ! vquirkd[v]->h ) {

                    s = nquirkd[u]->il;
                    nquirkdl[u] = nquirkd[u];
                    nquirkd[u] = nquirkdl[u]->h->d;

                    vquirkdl[v] = vquirkdl[v-1];
                    vquirkd[v] = vquirkd[v-1]->d;
                    break;
                }

                else if ( ! ( nquirkd[u]->h || vquirkd[v]->h ) ) {

                    nquirkdl[u] = nquirkdl[u-1];
                    nquirkd[u] = nquirkd[u-1]->d;
                    u--;

                    vquirkdl[v] = vquirkdl[v-1];
                    vquirkd[v] = vquirkd[v-1]->d;
                    v--;
                }
            }

            else if ( nquirkd[u]->il > vquirkd[v]->il ) {

                if ( nquirkd[u]->h ) {
                    s = nquirkd[u]->il;
                    nquirkdl[u] = nquirkd[u];
                    nquirkd[u] = nquirkdl[u]->h->d;
                    break;
                }

                else if ( ! nquirkd[u]->h ) {
                    nquirkdl[u] = nquirkdl[u-1];
                    nquirkd[u] = nquirkd[u-1]->d;
                    u--;
                }
            }

            else if ( vquirkd[v]->il > nquirkd[u]->il ) {

                if ( vquirkd[v]->h ) {
                    s = vquirkd[v]->il;
                    vquirkdl[v] = vquirkd[v];
                    vquirkd[v] = vquirkdl[v]->h->d;
                    break;
                }

                else if ( ! vquirkd[v]->h ) {
                    vquirkdl[v] = vquirkdl[v-1];
                    vquirkd[v] = vquirkd[v-1]->d;
                    v--;
                }
            }
        }
    }

    //  insert the lowest board number for which the QUIRK tree is checked
    v = n;
    if ( n == 0 )
        vquirkd[v] = tquirkd[d].h->d;
    else
        vquirkd[v] = nquirkd[n-1]->d;

    while ( vquirkd[v] ) {

        while ( vquirkd[v]->ic > ic ) {
            vquirkd[v]->ic = ic;
            if ( ! vquirkd[v]->d )
                break;
            v++;
            vquirkd[v] = vquirkd[v-1]->d;
        }

        while ( v > n && vquirkd[v]->h == NIL )
            v--;
        vquirkd[v] = vquirkd[v]->h ? vquirkd[v]->h->d : NIL;
    }

    //  update the lowest board nymber for the parent QUIRKD node
    v = n;
    if ( n == 0 ) {
        vquirkd[v] = tquirkd[d].h->d;
        while ( vquirkd[v] ) {
            tquirkd[d].ic = MAX( tquirkd[d].ic, vquirkd[v]->ic );
            vquirkd[v] = vquirkd[v]->h ? vquirkd[v]->h->d : NIL;
        }
    }
    else if ( ! nquirkd[n-1]->d )
        nquirkd[n-1]->ic = ic;
    else {
        vquirkd[v] = nquirkd[n-1]->d;
        while ( vquirkd[v] ) {
            nquirkd[n-1]->ic = MAX( nquirkd[n-1]->ic, vquirkd[v]->ic );
            vquirkd[v] = vquirkd[v]->h ? vquirkd[v]->h->d : NIL;
        }
    }
