# How SuitC works

## The model

In Gerrit van der Velde's words:

> SuitC is basically a single dummy solver. North-South only know their own
> hands and the probability of every possible East-West distribution.
> East-West know all hands and the North-South strategy.

So the defenders are treated as **double dummy**. They know declarer's
cards, and they know declarer's objective: if declarer goes for a safety play,
the defenders adjust their play to that goal. For many suit combinations the
defenders' best strategy is a *mixed* one, and SuitC assumes they always play
the optimal mixed strategy. The results are therefore a lower bound on what
declarer can expect against real defenders. This is the same model used by
Crowhurst, Roudinesco and Jeroen Warmerdam's SuitPlay.

A North-South strategy is fixed before the first card is played: it says how
North-South respond to every possible play by the defenders. SuitC never
re-evaluates probabilities during the play. Everything follows from the
a-priori probabilities of the distributions. For the same reason, you cannot
simply rerun SuitC after a trick with the remaining cards. The defenders'
mixed strategies and restricted choice make the conditional probabilities
differ from the a-priori ones. For restricted choice, SuitC treats touching
honours held by a defender (such as QJ) as "two queens", which builds the
principle in.

As Gerrit puts it, SuitC "loves overtricks and hates undertricks": among
strategies that are equally good for the chosen objective, it prefers the one
with the most expected tricks.

## The four phases

1. **Read the command line and build the East-West distributions.** Each
   distribution is weighted by its a-priori probability (or by vacant places
   with `-wv`/`-ev`). Equivalent small cards are merged.
2. **Build the complete game tree** of all possible plays: one routine per
   card of a trick (`create_ply1.c` … `create_ply4.c`, `create_tricks.c`).
   This phase "lacks any sophistication".
3. **Find all non-dominated strategies.** This is the heart of the program.
   The game tree is converted to a compact representation, the **QUIRK tree**
   (`convert_tree_to_quirk.c`). In this tree every node holds the list of
   strategies that are not dominated: no other strategy is at least as good on
   every distribution and better on one. The lists are merged bottom-up through
   the tree (`merge_quirk.c`, `clean_quirk.c`). They can grow to tens of
   millions of entries, which is why large combinations need so much memory,
   and why the `-c` cut-off exists. `-a` prints this list.
4. **Evaluate and print.** For each objective, the best strategy is picked from
   the list (`evaluate_best_results.c`). Strategies that tie are compared on
   expected tricks. The game tree of each optimal strategy is then printed,
   with equivalent branches folded together (`fold_ew_tree.c`,
   `fold_ns_tree.c`, `select_ns_plays.c`). `-F0` prints the full tree.

## Matchpoints and IMPs

At matchpoints the question is not how many tricks a strategy takes, but how
often it beats the other strategies. The opponents' choice of strategy is then
a game in its own right, and its solution can be a mixed strategy for
declarer. SuitC scores every non-dominated strategy against every other
(`evaluate_mp_scores.c`) and solves the resulting matrix game (`minimax.c`).
At matchpoints each distribution counts +1, 0 or −1, depending on which
strategy takes more tricks. At IMPs (`-I`), the difference between the
scores given for the two trick counts is converted to IMPs with the standard
IMP scale. `limit_matchpoint_nodes.c` bounds the size of the matrix
(`-M<nodes>`).

Matchpoint plays for suit combinations were introduced by Jeroen Warmerdam in
the Dutch magazine *IMP* in 1998.

## Weak domination (Zermelo)

`check_zermelo.c` can remove plays that are only weakly dominated (`-z1`).
`-z2` does this during the matchpoint evaluation instead.

## Choosing between equally good plays

Several strategies often achieve the same result. The printed tree then has to
pick one, and Gerrit notes that "there is always the problem of compactness of
the tree and the clarity of the representation". By default SuitC prefers
compact trees. With `-u` (Rev 2.009/2.010), it instead prefers the strategy
that wins its tricks as early as possible, needs the fewest entries outside
the suit, and uses the entries within the suit first. That is closer to how a
human declarer, or a bridge program like BEN, wants to play
(`use_suit_entries.c`).

## Entries and leads

By default North-South can lead from either hand at any time: they have
unlimited entries outside the suit. `-l<hand><n>` limits the number of
*free directed leads*, that is, entries to that hand from outside the suit.
`-L` fixes the opening leader, or the opening lead. Without `-L`, every
opening lead is tried.

## Code conventions

- One function per `.c` file, named after the function.
- All state lives in global variables, declared in `global.h`. `extern.h` is
  generated from `global.h` by the build, so never edit `extern.h`.
- A card set is a bit mask (`COLOR`, one bit per rank). The players are
  `west = 0, north = 1, east = 2, south = 3`.
- Memory comes from large pre-allocated pools (nodes, indices, quirks) with
  bump allocation. The pool sizes are set in `header.h`; see
  [Building](building.md#memory-limits).
