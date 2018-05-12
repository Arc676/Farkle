# Farkle

Library and command line frontend for Farkle dice game

For details regarding the functions available in the library, please consult the commented header file.

## License

Project source code available under GPLv3. See `LICENSE` for full license text.

## Contributing

Please follow established coding conventions and add a copyright line to the opening comments of each source file you modify.

## Gameplay

Farkle is a dice game in which the objective is to score as many points as possible. The number of turns is limited to an amount determined before the game begins. Players roll dice to score points in the following ways:
- A 1 is worth 100 points
- A 5 is worth 50 points
- Three of the same number are worth 100 times the number, except three 1s which are worth 1000 points instead; these dice must come up on the same roll, not separate ones (see Example 2)
- Each additional occurrence of a number beyond the first three increases the value of that set of dice by 100 times the number (the points obtained by rolling three copies of that number)

Players roll 6 dice and may select any subset of those dice to score points. Those dice are removed from the pool and the player may roll the remaining dice to score more points. If at any point none of the rolled dice can score points, the turn is considered a "farkle" and the player forfeits all their points from that turn. A player must remove at least one scoring die from the pool to reroll. A player may choose to stop rolling and "bank" any points they have already accumulated rather than reroll. Note that the probability of farkling is greater if the pool is smaller. If a player removes all 6 dice from the pool without farkling, that player keeps all their points from that turn in hand, puts all 6 dice back in the pool, and may reroll following the same rules as before. In this implementation, this is referred to as "cycling through the die pool." This may be done over any number of rolls (see example 3).

### Example 1

Player 1 rolls 6 dice and the result is
```
1 2 4 4 4 5
```
The total value of this roll is 550 points (1 - 100, (4 4 4) - 400, 5 - 50). Player 1 removes the three 4s and now has 400 points in hand. Player 1 chooses to reroll the remaining 3 dice and the result is
```
2 2 3
```
Player 1 has farkled because this roll is worth zero points. Player 1 forfeits the 400 points obtained from the previous roll and the turn passes on to the next player.

### Example 2

Player 1 rolls 6 dice and the result is
```
1 2 6 4 3 3
```
This roll is worth 100 points. Only the 1 is worth points; the others have no value. Player 1 removes the 1 and now has 100 points in hand. Player 1 chooses to reroll the remaining 5 dice and the result is
```
1 1 2 3 4
```
This roll is worth 200 points. Player 1 chooses to take both the 1s and now has 300 points in hand. The three 1s are *not* worth 1000 points because they were rolled on separate rolls. Player 1 chooses not to risk it and banks the 250 points. Player 1's score increases by 250 and the turn is passed to the next player.

### Example 3

Player 1 rolls 6 dice and the result is
```
1 1 1 4 6 6
```
This roll is worth 1000 points. Player 1 removes all three 1s from the pool and rolls again. The result is
```
1 5 5
```
This roll is worth 200 points. Because the fourth 1 was rolled in a new roll, it does not add to the value of the triple 1 from the previous roll. Player 1 removes all the dice from the pool and now has 1200 points in hand. The die pool has been exhausted but the player has not farkled. All six dice are put back in the pool and the player starts over, keeping the 1200 points obtained from the previous cycle. The roll is
```
1 1 5 3 4 4
```
This roll is worth 250 points. Player 1 removes the scoring dice and banks the 1450 points and the turn is passed to the next player. Note that if a player farkles after cycling through the die pool, that player still forfeits all their points.
