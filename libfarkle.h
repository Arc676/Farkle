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
	FIRST_ROLL = 0b0011,
	ROLLING    = 0b0010,
	PICKING    = 0b0100,
	TURN_ENDED = 0b1000
} GameState;

/**
 * Represents the success or failure of
 * toggling die selection
 */
typedef enum ToggleResult {
	PICKED,
	UNPICKED,
	NOT_PICKABLE,
	NOT_UNPICKABLE
} ToggleResult;

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
	char* name;
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
 * Determines the number of occurrences of each value
 * on pickable dice for the given roll
 * @param roll Pointer to roll state
 * @param values Array in which to store the value count
 */
void countDiceValues(Roll* roll, int* values);

/**
 * Determines which of the rolled dice can be
 * picked by the player based on their value
 * and how many times the number has appeared
 * @param roll Pointer to roll state
 * @param values Occurrences of values on pickable dice
 * @param allowed Array in which to store the acceptable dice (min size 6)
 */
void determinePickableDice(Roll* roll, int* values, int* allowed);

/**
 * Determines whether the roll was a farkle
 * @param roll Pointer to roll state
 * @return Whether any dice are worth points
 */
int isFarkle(Roll* roll);

/**
 * Toggles whether a die in the pool is selected
 * @param roll Pointer to the roll state
 * @param die Index of die whose selection state to toggle
 * @return Whether the toggle succeeded and why
 */
ToggleResult toggleDie(Roll* roll, int die);

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
 * the rolled dice; the selection struct is left in a
 * reusable state if the selection fails
 * @param roll Pointer to roll state
 * @param selection Pointer to struct in which selection data should be stored
 * @return Whether the selection was valid
 */
int constructSelection(Roll* roll, Selection* selection);

/**
 * Utility function for comparing two integer pointers
 * for qsort
 * @param a Pointer to first integer
 * @param b Pointer to second integer
 * @return Integer representation of whether a is greater
 */
int compareIntegers(const void* a, const void* b);

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
 * @param name Player name
 * @return Pointer to the new Player struct
 */
Player* createPlayer(char* name);

/**
 * Compares two players by their score
 * @param p1 Pointer to first player
 * @param p2 Pointer to second player
 * @return Integer representation of whether p1 has a higher score
 */
int comparePlayers(const void* p1, const void* p2);

/**
 * Sorts a list of players by score
 * @param players Player list
 * @param count Number of players
 */
void sortPlayers(Player** players, int count);

/**
 * Empties a hand
 * @param player Player whose hand to empty
 */
void emptyHand(Player* player);

/**
 * Banks all points currently in a given
 * player's hand
 * @param player Player whose hand to bank
 * @return The number of points banked
 */
int bankPoints(Player* player);
