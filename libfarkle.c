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

void initRoll(Roll* roll) {
	for (int i = 0; i < 6; i++) {
		roll->dice[i] = 0;
	}
}

void newRoll(Roll* roll) {
	for (int i = 0; i < 6; i++) {
		// negative values represent dice removed from the pool
		if (roll->dice[i] >= 0) {
			roll->dice[i] = rand() * 5 / RAND_MAX + 1;
		}
	}
}

int diePoolExhausted(Roll* roll) {
	for (int i = 0; i < 6; i++) {
		if (roll->dice[i] >= 0) {
			return 0;
		}
	}
	return 1;
}

int selectDice(Roll* roll, Hand* hand, int dice[]) {
}

void emptyHand(Hand* hand) {
	hand->timesSelected = 0;
}

void bankPoints(Player* player) {
	for (int i = 0; i < player->currentHand->timesSelected; i++) {
		player->score += player->currentHand->selections[i].value;
	}
	emptyHand(player->hand);
}

