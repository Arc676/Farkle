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

/**
 * Represents a player's roll
 */
typedef struct Roll {
	int dice[6];
} Roll;

/**
 * Represents a selection of dice from
 * the pool
 */
typedef struct Selection {
	int values[6];
	int value;
} Selection;

/**
 * Represents a player's hand with
 * all the dice selected this turn
 */
typedef struct Hand {
	Selection selections[10];
	int timesSelected;
} Hand;

/**
 * Represents a player
 */
typedef struct Player {
	Hand* currentHand;
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
 * Determines whether the die pool has been exhausted
 * @param roll Pointer to the roll state
 * @return Whether all the dice have been picked
 */
int diePoolExhausted(Roll* roll);

/**
 * Selects a subset of dice from the pool
 * and adds them to the given hand
 * @param roll Pointer to the roll state
 * @param hand Hand to which the dice should be added
 * @param dice[] List of indices of dice that should be selected
 * @return Whether the selection is valid
 */
int selectDice(Roll* roll, Hand* hand, int dice[]);

/**
 * Empties a hand
 * @param hand Hand to empty
 */
void emptyHand(Hand* hand);

/**
 * Banks all points currently in a given
 * player's hand
 * @param player Player whose hand to bank
 */
void bankPoints(Player* player);
