//Copyright (C) 2018 Arc676/Alessandro Vinciguerra

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation (version 3)

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//See README and LICENSE for more details

#include <stdlib.h>
#include <string.h>

/**
 * Represents the current state of the game
 * i.e. the action that the current player
 * must take
 */
typedef enum GameState {
	ROLLING,
	PICKING,
	TURN_ENDED
} GameState;

/**
 * Represents an individual die
 */
typedef struct Die {
	int value;
	int picked;
	int pickedThisRoll;
} Die;

/**
 * Represents a player's roll
 */
typedef struct Roll {
	Die dice[6];
} Roll;

/**
 * Represents a selection of dice from
 * the pool
 */
typedef struct Selection {
	int values[6];
	int dieCount;
	int value;
} Selection;

/**
 * Represents a player's hand with
 * all the dice selected this turn
 */
typedef struct Hand {
	Selection** selections;
	int selectionSize;
	int timesSelected;
} Hand;

/**
 * Represents a player
 */
typedef struct Player {
	Hand* hand;
	int score;
} Player;

/**
 * Initializes the roll state
 * @param roll Pointer to the roll state
 */
void initRoll(Roll* roll);

/**
 * Generates a new roll based on the
 * previous roll
 * @param roll Pointer to the roll state
 */
void newRoll(Roll* roll);

/**
 * Determines which of the rolled dice can be
 * picked by the player based on their value
 * and how many times the number has appeared
 * @param roll Pointer to roll state
 * @param allowed Array in which to store the acceptable dice (min size 6)
 */
void determinePickableDice(Roll* roll, int* allowed);

/**
 * Determines whether the roll was a farkle
 * @param roll Pointer to roll state
 * @return Whether any dice are worth points
 */
int isFarkle(Roll* roll);

/**
 * Removes a die from the pool
 * @param roll Pointer to the roll state
 * @param die Index of the die to remove
 * @return Whether the die was successfully removed
 */
int pickDie(Roll* roll, int die);

/**
 * Puts a die back into the pool
 * @param roll Pointer to the roll state
 * @param die Index of the die to put back
 * @return Whether the die was successfully put back
 */
int unpickDie(Roll* roll, int die);

/**
 * Determines whether the die pool has been exhausted
 * @param roll Pointer to the roll state
 * @return Whether all the dice have been picked
 */
int diePoolExhausted(Roll* roll);

/**
 * Constructs the die selection based on the state of
 * the rolled dice
 * @param roll Pointer to roll state
 * @param selection Pointer to struct in which selection data should be stored
 */
void constructSelection(Roll* roll, Selection* selection);

/**
 * Adds the given selection of dice to the given player's hand
 * @param player Player to whom the selection belongs
 * @param selection Die selection to add
 */
void appendSelection(Player* player, Selection* selection);

/**
 * Undoes the last selection appending to a player's hand
 * @param player Player from whose hand to remove the selection
 */
void undoSelection(Player* player);

/**
 * Unpicks all dice that were selected this roll
 * @param roll Pointer to roll state
 */
void deselectRoll(Roll* roll);

/**
 * Initializes a Player struct
 * @return Pointer to the new Player struct
 */
Player* createPlayer();

/**
 * Empties a hand
 * @param player Player whose hand to empty
 */
void emptyHand(Player* player);

/**
 * Banks all points currently in a given
 * player's hand
 * @param player Player whose hand to bank
 */
void bankPoints(Player* player);
