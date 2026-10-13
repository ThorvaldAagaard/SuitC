# Reading SuitC's output

This page uses the output of `SuitC -a KJ9 432 AQT8765` as its example.

## The header

```text
	North: KJ9
	South: 432
	West : AQT8765
	East : AQT8765

	min number of tricks: 0
	max number of tricks: 2
	number of plays     : 6
```

The header repeats the hands and the options in effect. `min`/`max number of
tricks` give the range of tricks North-South can take over all distributions.
`number of plays` is the number of strategies that are optimal for at least
one objective.

## The optimal results

```text
The optimal results
===================

	2 tricks:  24.7267
	1 tricks:  79.3913
	Result  :   1.0412
```

For each number of tricks, this section gives the best probability (in
percent) of taking **at least** that many tricks. `Result` is the highest
expected number of tricks. Different lines of play may achieve these
optima: here no single play gives both 24.73 % for two tricks and 79.39 % for
one trick.

## The optimum plays

```text
The optimum play for: MAX N1 N2
===============================

	2 tricks:  24.7267
	1 tricks:  54.6646
	0 tricks:  20.6087
	Result  :   1.0412
```

Each such section describes one strategy. The title lists the objectives for
which it is optimal:

| Label | Objective |
|---|---|
| `MAX` | maximum expected number of tricks |
| `N<k>` | safety play: the best chance of at least `<k>` tricks |
| `MP` | best play at matchpoints (with `-M`) |
| `IMP` | best play at IMPs (with `-I`) |

When several strategies tie for an objective, SuitC picks the one with the
most expected tricks. The figures under the title give the probability of
exactly `<k>` tricks for this strategy, followed by its expected number of
tricks (`Result`). With `-M` or `-I` the deviation from the optimal matchpoint
or IMP score is shown as well.

## The game tree

```text
                                               - 8 8 8 8 T T T T T Q Q Q ...
                                               8   8 8 8   8 8 8 8   8 8 ...
                                               ...
                                               A A A A A A A A A A A A A ...
S:4  A    9  T8-      S:4  Q     K  T8-     ns=2 2 2 2 2 2 2 2 2 2 - - - ...
                           T8-   J  QT8-    ns=2 2 2 2 2 2 2 2 2 - - 1 1 ...
     Q    K  AT8-     S:4  A     9  T8-     ns=2 2 2 2 2 2 2 2 2 2 - - - ...
```

### Columns: the East-West distributions

Each column on the right is one distribution of the East-West cards. Read a
column from top to bottom: first come West's cards, then a gap, then East's
cards. A `-` instead of the gap means that one defender is void: at the top,
West is void; at the bottom, East is void. Equivalent small cards are shown as
one symbol, so in this example `8` stands for any of 8, 7, 6 and 5.

### Rows: the play

The left part shows the play trick by trick. Each trick has four entries, in
playing order, starting with the leader:

- `S:4` — South leads the 4 (the hand that leads is shown before the colon);
- `A` — West plays the ace;
- `9` — North plays the 9;
- `T8-` — East plays the ten, a small card, or shows out (`-`).

The next trick follows on the same line (`S:4 Q K T8-`: South leads again,
West plays the queen, North the king, East follows). A line that starts
further to the right continues the line above it. It shows North-South's
response when the defenders play differently at that point. Together the lines
form North-South's complete strategy. An entry with several cards, such as
`QT8-`, means that any of those cards leads to the same continuation.

### `ns=`: the result per distribution

After each line, `ns=` gives the number of tricks North-South take for each
distribution (column) when play follows that line. A `-` means this line does
not occur for that distribution. Dots at the end of a line mean that no more
cards are played.

## The summary (`-a`)

```text
--------------------------------------------------------------------------------  -- 0 --  -- 1 --  -- 2 --  + R ++
 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 1 1 1 1 2 1 1 1 1 1 0 0 0 1 1 0 0 0 0 0  20.6087  54.6646  24.7267  1.0412
 1 1 1 1 1 1 1 1 1 2 2 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0 0 0 1 1 0 0 0 0 0  20.6087  78.1801   1.2112  0.8060
```

With `-a`, SuitC lists every strategy that is not dominated: no other
strategy is at least as good on every distribution and better on one. Each row
is one strategy. It gives the tricks taken on each distribution, the
probability of taking exactly 0, 1, 2, ... tricks, and the expected number of
tricks (`R`). The two rows above show the trade-off in this combination: the
first maximises the expected tricks and the chance of two tricks, while the
second is the safety play for one trick (79.39 %).

## The list of distributions (`-b`)

```text
       Probabilities    West       East       tricks
     ===============    ====       ====       ======
     1 x  0.26086957    KT98765    -           2 - 3
     6 x  0.48447205    KT9876     5           2 - 3
```

With `-b`, SuitC prints each distribution once, with the number of
equivalent distributions it stands for, the a-priori probability (in percent)
of each, and the lowest and highest number of tricks North-South take on it
across the possible lines of play.
