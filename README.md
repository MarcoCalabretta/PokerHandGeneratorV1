# PokerHandGeneratorV1
Calculates all possible texas hold'em hands resulting from every possible pair of hole cards.
So I like playing poker (mostly Texas Hold'em), and I wanted to improve as a player. A very important part of the game is what's called the preflop,
which is the round of betting after everyone has received their private (hole) cards but before any public (board) cards are dealt.
You get two cards and have to act based on how strong those two cards SHOULD be once all 5 public cards are dealt.
So, to evaluate the strength of my hole cards, I simulated getting every possible set of 5 public cards with every distinct set of 2 private cards I could have.
Distinct meaning I treated pairs of cards the same if they should have exactly the same strength (for example, Ace and Two of clubs is not distinct form Ace and Two of diamons).
The file runs, it takes about 20 minutes to print all the information to an output file, the problem is after finishing it I realized I didn't really know what to measure to evaluate the strength of my private cards anyway so it's sorta useless to me.
Enjoy!
