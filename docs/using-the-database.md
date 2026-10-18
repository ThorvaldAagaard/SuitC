# Using the SuitC database

*Gerrit A. van der Velde, Leiden*

This document contains a few suggestions how the SuitC database with the result of the analysis of all suit combinations can be used.

## Holdings with the same significant cards

The first suggestion is about generating reports about suit combinations with the same significant cards, but with a varying number of insignificant cards. Assume that directory `AllPlays` contains the analysis of all suit combinations. Execute the following commands:

```sh
cd AllPlays
mkdir View
cd View
v-equals > alist
```

The script `v-view` generates reports. Command `v-view -?` produces a list of parameters of `v-view`. Command

```sh
v-view -n AT -s Q -a -b -p alist
```

will generate a report with AT in north, T in south and varying numbers of insignificant cards in north and south.

The `-h`, `-n` and `-s` options are tricky. Their values should be contained in a line of the `alist` file. For instance, the command `v-view -n Q -s KT ...` is valid, but the command `v-view -n K -s QT ...` isn't.

## The playing strength of king and queen

The second suggestion is to run a series of `sc-strength` commands:

```sh
cd AllPlays
mkdir KQ
cd KQ
sc-strength -mn KQ -nn AJ -ns AJ        -ns AJ -r -o 34.KQ-0 ../34/34.xp
sc-strength -mn Q  -nn AJ -ns AJ -ms K  -ns AJ -r -o 34.Q-K  ../34/34.xp
sc-strength        -nn AJ -ns AJ -ms KQ -ns AJ -r -o 34.0-KQ ../34/34.xp
sc-strength -mn KQ -nn AJ -ns AJ        -ns AJ -r -o 25.KQ-0 ../25/25.xp
sc-strength -mn Q  -nn AJ -ns AJ -ms K  -ns AJ -r -o 25.Q-K  ../25/25.xp
sc-strength        -nn AJ -ns AJ -ms KQ -ns AJ -r -o 25.0-KQ ../25/25.xp
```

This series of commands creates 12 files with the playing strength of north (the `*.no` files) and south (the `*.so` files). Inspection of these files shows that with king and queen in a 7-card fit the playing strengths can vary widely, and that tens and nines can also have widely differing effects.

An alternative to investigate the effect of the placing of the cards is to use the `v-view` command. The command

```sh
v-view -h KJT -c7 -d[23][45] alist
```

produces a compact report on the playing strength.
