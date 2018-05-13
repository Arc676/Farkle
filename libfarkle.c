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

#include "libfarkle.h"

void initRoll(Roll* roll) {
	for (int i = 0; i < 6; i++) {
		roll->dice[i] = 0;
	}
}

void newRoll(Roll* roll) {
	for (int i = 0; i < 6; i++) {
		if (!roll->dice[i].picked) {
			roll->dice[i].value = rand() % 6 + 1;
		} else {
			roll->dice[i].pickedThisRoll = 0;
		}
	}
}

void determinePickableDice(Roll* roll, int* allowed) {
	memset(allowed, 0, 6);
	int values[6];
	for (int i = 0; i < 6; i++) {
		if (!roll->dice[i].picked) {
			values[roll->dice[i].value - 1]++;
		}
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

int isFarkle(Roll* roll) {
	int allowed[6];
	determinePickableDice(roll, allowed);
	for (int i = 0; i < 6; i++) {
		if (allowed[i]) {
			return 0;
		}
	}
	return 1;
}

int diePoolExhausted(Roll* roll) {
	for (int i = 0; i < 6; i++) {
		if (!roll->dice[i].picked) {
			return 0;
		}
	}
	return 1;
}

int pickDie(Roll* roll, int die) {
	int allowed[6];
	determinePickableDice(roll, allowed);
	if (allowed[die]) {
		roll->dice[die].picked = 1;
		roll->dice[die].pickedThisRoll = 1;
		return 1;
	}
	return 0;
}

int unpickDie(Roll* roll, int die) {
	if (roll->dice[die].picked) {
		if (roll->dice[die].pickedThisRoll) {
			roll->dice[die].picked = 0;
			roll->dice[die].pickedThisRoll = 0;
			return 1;
		}
	}
	return 0;
}

int selectDice(Roll* roll, Hand* hand, int dice[]) {
	return 0;
}

void constructSelection(Roll* roll, Selection* selection) {
	int dieCount = 0;
	int* values = selection->values;
	int chosenValues[6];
	memset(chosenValues, 0, sizeof(chosenValues));

	for (int i = 0; i < 6; i++) {
		if (roll->dice[i]->pickedThisRoll) {
			values[dieCount++] = roll->dice[i].value;
			chosenValues[roll->dice[i].value - 1]++;
		}
		// zero-indexed 1 and 5
		if (i != 0 && i != 4) {
			chosenValues[i] -= 2;
		}
	}
	int selectionValid = 1;
	for (int i = 1; i < 6; i++) {
		if (i == 4) {
			continue;
		}
		if (chosenValues[0] >= 3) {
			selection->value += (i + 1) * 100 * (chosenValues[i] - 2);
		} else if (chosenValues[0] > 0) {
			selectionValid = 0;
			break;
		}
	}
	if (!selectionValid) {
		selection->value = 0;
		return;
	}
	selection->dieCount = dieCount;
	if (chosenValues[0] >= 3) {
		selection->value += 1000 * (chosenValues[0] - 2);
	} else {
		selection->value += 100 * chosenValues[0];
	}
	if (chosenValues[4] >= 3) {
		selection->value += 500 * (chosenValues[4] - 2);
	} else {
		selection->value += 50 * chosenValues[0];
	}
}

void appendSelection(Player* player, Selection* selection) {
	Hand* hand = player->currentHand;
	hand->selections[hand->timesSelected++] = selection;
}

void emptyHand(Hand* hand) {
	hand->timesSelected = 0;
}

void bankPoints(Player* player) {
	for (int i = 0; i < player->currentHand->timesSelected; i++) {
		player->score += player->currentHand->selections[i]->value;
	}
	emptyHand(player->currentHand);
}

