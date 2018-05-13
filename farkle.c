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
#include <string.h>
#include <time.h>

#include "libfarkle.h"

Player** players;
int pCount = 1;
int turns = 10;

void printHelp() {
	printf("help - show this help text\n\
roll - roll die pool\n\
view - view the current roll\n\
pick - pick dice from die pool\n\
hand - show your current hand\n\
bank - bank all points currently in hand\n\
exit - exit the game immediately\n");
}

void viewRoll(Roll* roll) {
	printf("Your roll:\n");
	for (int i = 1; i <= 6; i++) {
		printf("%d ", i);
	}
	printf("\n------------\n");
	for (int i = 0; i < 6; i++) {
		if (roll->dice[i].picked) {
			printf("- ");
		} else {
			printf("%d ", roll->dice[i]);
		}
	}
	printf("\n");
}

void playGame() {
	char cmd[100];
	Roll* roll = (Roll*)malloc(sizeof(Roll));
	// play for given number of turns
	for (int turn = 0; turn < turns; turn++) {
		// each player gets a turn
		for (int player = 0; player < pCount; player++) {
			initRoll(roll);
			GameState state = ROLLING;
			// until player banks or farkles
			while (state != TURN_ENDED) {
				printf("%d> ", player + 1);
				fgets(cmd, sizeof(cmd), stdin);
				cmd[strlen(cmd) - 1] = 0;
				if (!strcmp(cmd, "help")) {
					printHelp();
				} else if (!strcmp(cmd, "bank")) {
					bankPoints(players[player]);
					state = TURN_ENDED;
				} else if (!strcmp(cmd, "exit")) {
					// set all loop limits and exit
					player = pCount;
					turn = turns;
					state = TURN_ENDED;
				} else if (!strcmp(cmd, "roll")) {
					if (state == PICKING) {
						printf("You have already rolled. Type 'pick' to pick from the die pool.\n");
					} else {
						newRoll(roll);
						viewRoll(roll);
						if (isFarkle(roll)) {
							printf("Farkle!\n");
							emptyHand(players[player]->hand);
							state = TURN_ENDED;
						} else {
							state = PICKING;
						}
					}
				} else if (!strcmp(cmd, "view")) {
					if (state == ROLLING) {
						printf("You have not rolled yet. Type 'roll' to roll the die pool.\n");
					} else {
						viewRoll(roll);
					}
				} else if (!strcmp(cmd, "pick")) {
					printf("Enter die index to pick or unpick. Enter a value greater than 6 to stop picking.\n");
					int index = 0;
					char input[10];
					for (;;) {
						printf("Picking> ");
						fgets(input, sizeof(input), stdin);
						index = atoi(input);
						if (index < 1 || index > 6) {
							break;
						}
						if (roll->dice[index - 1].picked) {
							if (unpickDie(roll, index - 1)) {
								printf("Unpicked die %d\n", index);
							} else {
								printf("You cannot unpick this die\n");
							}
						} else {
							if (pickDie(roll, index - 1)) {
								printf("Picked die %d\n", index);
							} else {
								printf("You cannot pick this die\n");
							}
						}
					}
					Selection* sel = (Selection*)malloc(sizeof(Selection));
					constructSelection(roll, sel);
					if (sel->value > 0) {
						printf("Selected %d points' worth of dice.\n", sel->value);
						state = ROLLING;
						appendSelection(players[player], sel);
					} else {
						printf("The selection is invalid\n");
						for (int i = 0; i < 6; i++) {
							unpickDie(roll, i);
						}
					}
				} else {
					printf("Invalid command '%s'. Type 'help' to see a list of commands.\n", cmd);
				}
				memset(cmd, 0, sizeof(cmd));
			}
		}
	}
	printf("Game over\n");
}

int main(int argc, char* argv[]) {
	srand(time(NULL));
	int opt;
	while ((opt = getopt(argc, argv, "p:t:")) != -1) {
		switch (opt) {
			case 'p':
				pCount = atoi(optarg);
				break;
			case 't':
				turns = atoi(optarg);
				break;
			default:
				fprintf(stderr, "Usage: farkle [-p player_count] [-t turn_limit]\n");
				return 1;
		}
	}
	// create players
	players = (Player**)malloc(pCount * sizeof(Player*));
	for (int i = 0; i < pCount; i++) {
		players[i] = createPlayer();
	}
	// play game
	playGame();
	// free player structs
	for (int i = 0; i < pCount; i++) {
		free(players[i]);
	}
	free(players);
	return 0;
}
