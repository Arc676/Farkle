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

#include <stdio.h>
#include <unistd.h>

#include "libfarkle.h"

Player** players;

void playGame() {}

int main(int argc, char* argv[]) {
	int opt, pCount = 1;
	while ((opt = getopt(argc, argv, "p:")) != -1) {
		switch (opt) {
			case 'p':
				pCount = atoi(optarg);
				break;
			default:
				fprintf(stderr, "Usage: farkle [-p player_count]\n");
				return 1;
		}
	}
	players = (Player**)malloc(pCount * sizeof(Player*));
	for (int i = 0; i < pCount; i++) {
		players[i] = (Player*)malloc(sizeof(Player));
	}
	playGame();
	return 0;
}
