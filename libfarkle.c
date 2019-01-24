//Copyright (C) 2018-9 Arc676/Alessandro Vinciguerra

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

#include "libfarkle.h"

void initRoll(Roll* roll) {
	for (int i = 0; i < 6; i++) {
		Die die;
		die.picked = 0;
		die.pickedThisRoll = 0;
		die.value = 0;
		roll->dice[i] = die;
	}
}

void newRoll(Roll* roll) {
	if (diePoolExhausted(roll)) {
		initRoll(roll);
	}
	for (int i = 0; i < 6; i++) {
		if (roll->dice[i].picked) {
			roll->dice[i].pickedThisRoll = 0;
		} else {
			roll->dice[i].value = rand() % 6 + 1;
		}
	}
}

void countDiceValues(Roll* roll, int* values) {
	memset(values, 0, 6 * sizeof(int));
	for (int i = 0; i < 6; i++) {
		if (!roll->dice[i].picked || roll->dice[i].pickedThisRoll) {
			values[roll->dice[i].value - 1]++;
		}
	}
}

void determinePickableDice(Roll* roll, int* values, int* allowed) {
	for (int i = 0; i < 6; i++) {
		// zero-indexed 1 and 5; values that can be picked
		// even if they only appear once
		if (i != 0 && i != 4) {
			values[i] -= 2;
		}
	}
	for (int i = 0; i < 6; i++) {
		allowed[i] = !roll->dice[i].picked && values[roll->dice[i].value - 1] > 0;
	}
}

RollType determineRollType(Roll* roll, Selection* selection) {
	int values[6];
	countDiceValues(roll, values);

	//whether the roll is a straight
	int isStraight = 1;

	//whether the roll is a triple pair
	int isTriplePair = 1;
	//number of times a pair has appeared
	int _2s = 0;

	for (int i = 0; i < 6; i++) {
		if (isStraight && values[i] != 1) {
			isStraight = 0;
		}
		if (isTriplePair) {
			if (values[i] == 2) {
				_2s++;
			} else if (values[i] != 0) {
				isTriplePair = 0;
			}
		}
		if (!isTriplePair && !isStraight) {
			break;
		}
	}
	if (isStraight || (isTriplePair && _2s == 3)) {
		for (int i = 0; i < 6; i++) {
			selection->values[i] = roll->dice[i].value;
			roll->dice[i].picked = 1;
			roll->dice[i].pickedThisRoll = 1;
		}
		qsort(selection->values, 6, sizeof(int), compareIntegers);
		selection->value = isStraight ? 3000 : 2000;
		selection->dieCount = 6;
		return isStraight ? STRAIGHT : TRIPLE_PAIR;
	}

	int allowed[6];
	determinePickableDice(roll, values, allowed);
	for (int i = 0; i < 6; i++) {
		if (allowed[i]) {
			return SIMPLE;
		}
	}
	return FARKLE;
}

int diePoolExhausted(Roll* roll) {
	for (int i = 0; i < 6; i++) {
		if (!roll->dice[i].picked) {
			return 0;
		}
	}
	return 1;
}

ToggleResult toggleDie(Roll* roll, int die) {
	if (roll->dice[die].picked) {
		return unpickDie(roll, die) ? UNPICKED : NOT_UNPICKABLE;
	} else {
		return pickDie(roll, die) ? PICKED : NOT_PICKABLE;
	}
}

int pickDie(Roll* roll, int die) {
	int values[6];
	countDiceValues(roll, values);

	int allowed[6];
	determinePickableDice(roll, values, allowed);
	if (allowed[die]) {
		roll->dice[die].picked = 1;
		roll->dice[die].pickedThisRoll = 1;
		return 1;
	}
	return 0;
}

int unpickDie(Roll* roll, int die) {
	if (roll->dice[die].pickedThisRoll) {
		roll->dice[die].picked = 0;
		roll->dice[die].pickedThisRoll = 0;
		return 1;
	}
	return 0;
}

int compareIntegers(const void* a, const void* b) {
	return *(int*)a - *(int*)b;
}

int constructSelection(Roll* roll, Selection* selection) {
	int dieCount = 0;
	int chosenValues[6];
	memset(chosenValues, 0, 6 * sizeof(int));

	// initialize selection
	selection->value = 0;
	selection->dieCount = 0;

	for (int i = 0; i < 6; i++) {
		if (roll->dice[i].pickedThisRoll) {
			selection->values[dieCount++] = roll->dice[i].value;
			chosenValues[roll->dice[i].value - 1]++;
		}
	}
	int selectionValid = 1;
	for (int i = 1; i < 6; i++) {
		if (i == 4) {
			continue;
		}
		if (chosenValues[i] >= 3) {
			selection->value += (i + 1) * 100 * (chosenValues[i] - 2);
		} else if (chosenValues[i] > 0) {
			selectionValid = 0;
			break;
		}
	}
	if (!selectionValid) {
		return 0;
	}
	qsort(selection->values, dieCount, sizeof(int), compareIntegers);
	selection->dieCount = dieCount;
	if (chosenValues[0] >= 3) {
		selection->value += 1000 * (chosenValues[0] - 2);
	} else {
		selection->value += 100 * chosenValues[0];
	}
	if (chosenValues[4] >= 3) {
		selection->value += 500 * (chosenValues[4] - 2);
	} else {
		selection->value += 50 * chosenValues[4];
	}
	return selection->value > 0;
}

int appendSelection(Player* player, Selection* selection) {
	Hand* hand = player->hand;
	hand->selections[hand->timesSelected++] = selection;
	if (hand->timesSelected >= hand->selectionSize) {
		Selection** newSel = (Selection**)realloc(hand->selections, hand->selectionSize * 2 * sizeof(Selection*));
		if (newSel == NULL) {
			return 0;
		}
		hand->selections = newSel;
		hand->selectionSize *= 2;
	}
	return 1;
}

void undoSelection(Player* player) {
	player->hand->timesSelected--;
}

void deselectRoll(Roll* roll) {
	for (int i = 0; i < 6; i++) {
		unpickDie(roll, i);
	}
}

Player* createPlayer(char* name) {
	Player* p = (Player*)malloc(sizeof(Player));
	p->score = 0;
	p->name = name;
	p->hand = (Hand*)malloc(sizeof(Hand));
	p->hand->timesSelected = 0;
	p->hand->selectionSize = 10;
	p->hand->selections = (Selection**)malloc(p->hand->selectionSize * sizeof(Selection*));
	return p;
}

int comparePlayers(const void* p1, const void* p2) {
	int s1 = (*(Player**)p1)->score;
	int s2 = (*(Player**)p2)->score;
	if (s1 > s2) {
		return -1;
	}
	if (s1 < s2) {
		return 1;
	}
	return 0;
}

void sortPlayers(Player** players, int count) {
	qsort(players, count, sizeof(Player*), comparePlayers);
}

void emptyHand(Player* player) {
	for (int i = 0; i < player->hand->timesSelected; i++) {
		free(player->hand->selections[i]);
	}
	player->hand->timesSelected = 0;
}

int bankPoints(Player* player) {
	int total = 0;
	for (int i = 0; i < player->hand->timesSelected; i++) {
		total += player->hand->selections[i]->value;
	}
	player->score += total;
	emptyHand(player);
	return total;
}

void freePlayer(Player* player) {
	free(player->name);
	emptyHand(player);
	free(player->hand->selections);
	free(player->hand);
	free(player);
}
