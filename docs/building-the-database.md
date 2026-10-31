# The SuitC database with all suit combinations

*Gerrit A. van der Velde, Leiden*

The SuitC environment comes with scripts to analyse all suit combinations. The first step is to create for each distribution a script with commands to analyse each suit combination in turn. This is done with the script `sc-install`, which is started with the command `sc-install [-M] [<c>]`. This script starts with a call to program `playc`. This program generates all distributions with the following restriction. If the north and south hand contain a sequence of cards only the distribution is included in which the north hand contains the lower cards and the south hand the higher ones.

The second step in the `sc-install` script is to distribute this list of suit combination over the files `??/??`. So file `35/35` contains the list of all suit combinations with 3 cards in north and 5 cards in south.

The third step is to convert these files to files with SuitC commands. These command files can be modified. Calling `sc-install` with the `-M` parameter selectively adds the `-M` parameter to the commands in the `??/??` files to perform a matchpoint analysis. Calling `sc-install` with the `c` parameter adds a `-c` parameter to specified commands. Most suit combinations can be analysed without using the `-c` parameter, but a couple of suit combinations require resources that are beyond the capacity of a laptop. The `-c` parameter restricts the use of resources.

A full analysis of all suit combinations is much faster if you install the scripts with the command `sc-install 020k`. However, to figure out the minimal settings for the `-c` and the `-M` parameters costs a week or so.

Once the environment is built the analysis is done with the command `sc-runit ??` where `??` stands for a distribution like 35 or 44. The `sc-runit` script uses Perl scripts `P-run` and `P-resultm` to perform the analysis to create files `??/res` with the results of the analysis. These scripts keep track of insignificant cards to avoid re-analysing a suit combination if the insignificant cards have no effect. After finishing the analysis of all suit combinations in a directory script `sc-install` executes a couple of scripts to adjust the output.
