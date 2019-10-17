/*
 * Name: Jordan Muehlbauer
 * Date: 10/16/19
 * Description: Headerfile to support all the functions needed for Yahtzee
 */

#ifndef PA5A2_SOURCE_H
#define PA5A2_SOURCE_H

typedef struct Player {
    int scores[13];
} Player;

void openMenu();

void printMenu();

void printRules();

void newGame();

void turn(Player* player);

void printPlayerDice(int dice[5]);

int askYesOrNoQUestion(char str[]);

int canUserUseScore(int scores[13], int spot);

void printPlayerScoreSheet(int scores[13]);

void roll(int dice[5], int reroll[5]);

void applyScore(int scores[13], int dice[5], int spot);

void reroll(int dice[5], int spot);

int calculateTotalScore(Player p);

void finishGame(Player p1, Player p2);

void helpfulRerollMenu(int dice[5], int reroll[5]);

int doesArrayContainInt(int value, int array[5]);

int isYahtzee(int dice[5]);

int isSmallStraight(int dice[5]);

int isLargeStraight(int dice[5]);

int isFullhouse(int dice[5]);

int isThreeOfAKind(int dice[5]);

int isFourOfAKind(int dice[5]);


#endif //PA5A2_SOURCE_H
