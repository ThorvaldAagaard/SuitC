# Hand Valuation and Computers

*Gerrit A. van der Velde*

By far the most popular method of hand valuation is the Milton Work point count method with adjustments for shortages. It is known that the method is far from accurate, but for human players it has one overwhelming advantage: it is easy to apply even for not too experienced players.

In my opinion there are better methods. Computers can handle complicated evaluations based on extensive tables in a matter of seconds.

My idea is to use the results of single dummy analysis of suit combinations as the backbone of for the evaluation of the playing strength of a bridge hand.

Examples if single dummy suit analysis can be found in the lemma “Suit Combination” written by Eric Crowhusrst in “The Official Encyclopedia of Bridge”, in J.M. Roudinesco's  book “The Dictionary of Suit Combinations” or in Jerooen Warmerdam's computer program “SuitPlay”.

A few remarks on single dummy analysis of suit combinations are in order. The method used by Crowhurst, Roudinesco and Warmerdam is single dummy only for the leader. For the defenders the method is really double dummy as it is supposed that the cards of the leader are known to the defenders. Moreover, it is assumed that the defenders know the goal of the leader. If the leader applies a safety play for a specfied number of tricks instead of aiming for the maximum number of tricks, the defenders will adjust their play flawlessly to the new goal. For many suit combination the optimal strategy for the defenders is a mixed strategy, but is assumed that they will always use the optimal mixed stategy.

For the above reasons the result of single dummy analysis of suit combinations are the lower limit for the results obtainable for the leader. However, an analysis based on single dummy analysis for the leader as well as for the defenders is completely unfeasible as it would mean that the anlysis of all suit combinations should be combined in a huge single run.

I have used my computer program SuitC to create a database with the the result of the analysis of all suit combinations. This database contains for every suit combination the North and South hands, the expected number of tricks won by the leader and the probability to win a specified number of tricks. The analysis of the suit combinations is in this database based on the assumption that North and South have enough entrees to lead always from the optimal hand. SuitC is flexible regarding additional options, as it allows restrictions on the number of tricks played, the number of entries, the minimum number of cards in East and/or West, to specify that East and/or West hold specified cards, to specify the number of vacant place in East and/or West, the number of trumps in specified hands. It even contains an option to lead from East or West at every trick.

Hand valuation has different aspects, such as timing, control and estimatiing how many tricks are played in each suit. The SuitC databases come into play when it is known how many tricks are played in the analysed suit.

Jeff Rubens mentions in his book “The secrets of Winning Bridge” several factors that influence the strength of a hand. As a starter we will try to answer the issue. A partnership owns a combined holding of high cards. Is it better to have these cards in the same hand or to have these cards divided over both hands?

I analyzed the situation that the partnership has the king and queen in a suit and a combined holding of seven cards in the suit, divided 2-5, 3-4, 4-3 or 5-3. The reults are:

### 2-5 distribution, divided honors

```text
South    #tricks    1 trick   2 tricks   3 tricks   4 tricks
=====    =======   ========   ========   ========   ========
Q5432     2.3717   100.0000    87.7578    46.8323     3.5528
Q9432     2.5282   100.0000    91.2050    57.6522     5.1677
QT432     2.8192   100.0000    94.9503    67.6646    20.6708
QT932     3.1851   100.0000   100.0000    90.5714    27.9379
QT982     3.2988   100.0000   100.0000   100.0000    29.8758
```

### 2-5 distribution, South has both honors

```text
South    #tricks    1 trick   2 tricks   3 tricks   4 tricks
=====    =======   ========   ========   ========   ========
KQ432     2.7197   100.0000    92.9255    61.9048    17.7640
KQ932     2.8632   100.0000    96.3106    71.6236    19.8095
KQT32     2.9229   100.0000    97.5217    75.6501    20.8861
KQT92     3.3818   100.0000   100.0000    95.7888    42.3913
KQT98     3.4320   100.0000   100.0000   100.0000    43.1988
```

For a 2-5 distribution it is clearly better to have both honors in the long hand. However, intermediate cards in the long hand are even more important. The holding of QT982 is nearly a full trick stronger than the holding Q5

### 3-4 distribution, divided honrs

```text
South    #tricks    1 trick   2 tricks   3 tricks
=====    =======   ========   ========   ========
Q432      1.7275   100.0000    64.2696     8.9168
Q932      1.9464   100.0000    80.2746    14.9607
QT32      2.1096   100.0000    88.2899    26.6419
QT92      2.5283   100.0000   100.0000    52.8261
```

### 3-4 distribution, South has both honors

```text
South    #tricks    1 trick   2 tricks   3 tricks
=====    =======   ========   ========   ========
KQ32      1.9602   100.0000    80.6566    19.7578
KQ92      2.1997   100.0000    90.1430    31.3769
KQT2      2.3307   100.0000    93.2760    40.7143
KQT9      2.5121   100.0000   100.0000    51.2112
```

The picture is the same as for the 2-5 distribution. Notice that the playing strength of the 2-5 distribution is about 0.8 trick better than that of the 3-4 distribution.

### 4-3 distribution, divided honors

```text
South    #tricks    1 trick   2 tricks   3 tricks
=====    =======   ========   ========   ========
Q32       1.8717   100.0000    72.8563    16.5484
Q92       2.0563   100.0000    86.0557    20.0296
QT2       2.2139   100.0000    91.6895    30.5271
QT9       2.5121   100.0000   100.0000    51.2112
```

### 4-3 distribution, South has both honors

```text
South    #tricks    1 trick   2 tricks   3 tricks
=====    =======   ========   ========   ========
KQ2       2.1141   100.0000    86.9965    24.4082
KQ9       2.1751   100.0000    89.5342    27.9725
KQT       2.1892   100.0000    90.9472    27.9725
```

The sailant feature is that the holding KQT is not significantly stronger than KQ2.

### 5-2 distribution, divided honors

```text
South    #tricks    1 trick   2 tricks   3 tricks   4 tricks
=====    =======   ========   ========   ========   ========
Q2        2.7186   100.0000    94.9823    65.6220    12.3540
Q9        2.8592   100.0000    97.3886    75.2280    14.2458
QT        3.0029   100.0000    98.1460    79.7959    23.1278
```

### 5-2 distribution, South has both honors

```text
South    #tricks    1 trick   2 tricks   3 tricks   4 tricks
=====    =======   ========   ========   ========   ========
KQ        2.6429   100.0000    93.1810    65.3120     5.7996
```

With a 5-2 distribution the holding of KQ sec is much weaker than divided honors, as one of the honors will fall under the ace.

## Conclusion

We notice the huge variation in playing strength for different location of the honor cards, for differences in distribution and for holding intermediate cards even if the number of cards for the partnership is the same. The expected number of tricks ranges from 1.73 to 3.43!
