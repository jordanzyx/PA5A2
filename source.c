/*
 * Name: Jordan Muehlbauer
 * Date: 10/16/19
 * Description: Implementation of source.h to handle Yahtzee Text Edition!
 */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "source.h"

#define PRINT_RULES 1
#define START_GAME 2
#define EXIT 3

void openMenu() {
    //Allow for us to use the rand() function properly
    srand(time(NULL));
    //Handle player decisions in the menu
    int choice = 0;
    //Menu loop
    while (choice != EXIT) {
        system("cls");
        printMenu();
        scanf("%d", &choice);
        if (choice == PRINT_RULES)printRules();
        if (choice == START_GAME)newGame();
    }
}

void printMenu() {
    printf("1. Print game rules\n");
    printf("2. Start a game of Yahtzee\n");
    printf("3. Exit\n");
}

void printRules() {
    //Clear the screen
    system("cls");
    //Print link to the rules
    printf("View rules at https://www.yahtzeeonline.org/yahtzee-rules.php");
}

void newGame() {
    //Clear the screen
    system("cls");

    //Create two players
    Player p1 = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} };
    Player p2 = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} };

    //Assign pointer variables for each so we can write to them from different scopes
    Player* p1Ptr = &p1;
    Player* p2Ptr = &p2;

    //Run 13 rounds of Yahtzee
    for (int i = 0; i < 13; ++i) {
        turn(p1Ptr);
        turn(p2Ptr);
    }

    //Wrap up the game
    finishGame(p1, p2);
}

void finishGame(Player p1, Player p2) {
    //collect the players scores
    int p1Score = calculateTotalScore(p1);
    int p2Score = calculateTotalScore(p2);

    //Display scores
    printf("Player 1 scored %d\n", p1Score);
    printf("Player 2 scored %d\n", p2Score);

    //Inform players of tie
    if (p1Score == p2Score) {
        printf("Therefore the game ended in a tie!\n");
        system("pause");
        return;
    }

    //Inform players of who won
    if (p1Score > p2Score)printf("Therefore Player 1 won the game!\n");
    else printf("Therefore Player 2 won the game!\n");
    system("pause");
}

int calculateTotalScore(Player p) {
    //initialize a variable to return the players total score
    int totalScore = 0;

    //Loop through all of the score slots on the players scoresheet
    for (int i = 0; i < 13; ++i) {

        //After the first 6 scores have been added up check if they get the bonus
        if (i == 6 && totalScore > 63)totalScore += 35;

        //Find the current score at position i
        int score = p.scores[i];

        //If the score was scratched continue
        if (score == -1)continue;

        //increase the total score
        totalScore += score;
    }

    return totalScore;
}


void printPlayerDice(int dice[5]) {
    printf("Dice #1 %d\nDice #2 %d\nDice #3 %d\nDice #4 %d\nDice #5 %d\n", dice[0], dice[1], dice[2], dice[3], dice[4]);
}

void turn(Player* player) {
    //Initialize turn based variables
    int rolls = 0;
    int dice[5] = { 0, 0, 0, 0, 0 };
    int reroll[5] = { 1, 1, 1, 1, 1 };
    int scored = 0;
    //Allow the player to keep rolling until they hit 3 turns
    while (rolls < 3 && scored != 1) {

        //Prompts the player
        printf("Starting to roll!\n");
        system("pause");

        //Roll the players dice
        roll(dice, reroll);

        //Print out their role for them to see
        printPlayerDice(dice);

        //Increase the amount of player rolls!
        rolls++;

        //Ask the player if they would like to use their current role on their scoresheet given that they have not used up all their rolls already
        int useRoll = 1;
        if (rolls < 3)useRoll = askYesOrNoQUestion("Would you like to use this roll on your scoresheet?");

        //Either apply score or Allow them to roll again
        if (useRoll == 1) {
            //Print their scoresheet so they can see what options they have avaliable
            printPlayerScoreSheet(player->scores);

            int score = -1;
            while (score < 1 || score > 13) {
                //Prompt User
                printf("Which score would you like to use your current hand on?\n");

                //Collect
                scanf(" %d", &score);

                //Retry if they didnt enter a valid number
                if (score < 1 || score > 13)continue;

                //Check if player can use that score
                int canUseScore = canUserUseScore(player->scores, score - 1);

                //Either apply score or ask them again what score they want to use
                if (canUseScore == 1) {
                    //Apply the score
                    applyScore(player->scores, dice, score - 1);

                    //Peacefully exit the turn
                    scored = 1;
                }
                else {
                    //Inform user that they cannot use that score spot
                    printf("Sorry you cannot do that with your current scoresheet\n");
                    //Reset score-spot to an invalid response
                    score = -1;
                }


            }

        }

        //If the user has not scored allow them to decide what dice to reroll
        if (!scored) {
            int picked = 0;
            //Reset reroll array
            for (int i = 0; i < 5; ++i) {
                reroll[i] = 1;
            }

            while (picked != 4) {
                //Display helpful menu
                helpfulRerollMenu(dice, reroll);

                //Ask the user what dice they would like to not reroll
                printf("Which dice's re-roll status would you like change? [1-5]? You can chose up to %d to not re-roll. To stop enter 6\n",
                       4 - picked);

                int spot = 0;
                scanf("%d", &spot);

                //If the user has entered the code to reroll allow them to
                if (spot == 6)break;

                //If the requested dice position is invalid re-ask the question
                if (spot < 1 || spot > 5)continue;

                //Remove the roll from the reroll position or Add the roll back
                int rolling = reroll[spot - 1];

                //Adjust the reroll Array
                reroll[spot - 1] = -rolling;

                //Increase / Decrease the amount of dice selected for reroll based on the operation above
                picked += rolling;

                //Clear screen
                system("cls");
            }
        }
    }

    //Clear the screen
    system("cls");
}

void helpfulRerollMenu(int dice[5], int reroll[5]) {
    for (int i = 0; i < 5; ++i) {
        printf("Dice #%d %d, Rerolling [%c]\n", (i + 1), dice[i], reroll[i] == 1 ? 'Y' : 'N');
    }
}

void applyScore(int scores[13], int dice[5], int spot) {
    if (spot < 6) {
        //Collect the score for the Sum score spots
        int die = spot + 1;
        int score = 0;
        //Look through all the dice
        for (int i = 0; i < 5; ++i) {
            if (dice[i] == die)score += die;
        }
        //If they have none of the dice scratch the score, otherwise give them the proper score
        scores[spot] = score == 0 ? -1 : score;

        return;
    }

    //Determine if the player has to scratch
    int scratch = 0;
    if (spot == 6)scratch = isThreeOfAKind(dice) == 1 ? 1 : -1;
    if (spot == 7)scratch = isFourOfAKind(dice) == 1 ? 1 : -1;
    if (spot == 8)scratch = isFullhouse(dice) == 1 ? 1 : -1;
    if (spot == 9)scratch = isSmallStraight(dice) == 1 ? 1 : -1;
    if (spot == 10)scratch = isLargeStraight(dice) == 1 ? 1 : -1;
    if (spot == 11)scratch = isYahtzee(dice) == 1 ? 1 : -1;

    //Handle the scores where it is the sum of all the dice
    if (spot == 6 || spot == 7 || spot == 12) {
        scores[spot] = scratch == 1 ? dice[0] + dice[1] + dice[2] + dice[3] + dice[4] : -1;
        return;
    }
    //Handle Fullhouse
    if (spot == 8)scores[spot] = scratch == 1 ? 25 : -1;

    //Handle Small Straight
    if (spot == 9)scores[spot] = scratch == 1 ? 30 : -1;

    //Handle Large Straight
    if (spot == 10)scores[spot] = scratch == 1 ? 40 : -1;

    //Handle Yahtzee
    if (spot == 11)scores[spot] = scratch == 1 ? 50 : -1;
}


int askYesOrNoQUestion(char question[]) {

    //Create variable to store the user answer
    char c = 'a';

    //Prompt the users response until they give a valid response
    while (c != 'y' && c != 'Y' && c != 'n' && c != 'N') {

        //Ask the user the question
        printf("%s[y/n]\n", question);

        //Colect the answer
        scanf(" %c", &c);
    }

    //Return true / false
    return c == 'y' || c == 'Y' ? 1 : -1;
}

void reroll(int dice[5], int spot) {
    dice[spot] = (rand() % 6) + 1;
}

void roll(int dice[5], int bools[5]) {
    //For every dice roll them a new value
    for (int i = 0; i < 5; ++i) {
        //Check if the dice is set to be rerolled
        int roll = bools[i];
        //Reroll dice if true
        if (roll == 1)reroll(dice, i);
    }
}

void printPlayerScoreSheet(int scores[13]) {
    printf("------ Your Scoresheet ------\n");
    printf("1. Sum of 1's : %d\n", scores[0]);
    printf("2. Sum of 2's : %d\n", scores[1]);
    printf("3. Sum of 3's : %d\n", scores[2]);
    printf("4. Sum of 4's : %d\n", scores[3]);
    printf("5. Sum of 5's : %d\n", scores[4]);
    printf("6. Sum of 6's : %d\n", scores[5]);
    printf("-----------------------------\n");
    printf("7. Three of a Kind : %d\n", scores[6]);
    printf("8. Four of a Kind : %d\n", scores[7]);
    printf("9. Full house : %d\n", scores[8]);
    printf("10. Small straight : %d\n", scores[9]);
    printf("11. Large straight : %d\n", scores[10]);
    printf("12. Yahtzee : %d\n", scores[11]);
    printf("13. Chance : %d\n", scores[12]);
}

int canUserUseScore(int scores[13], int spot) {
    //Check if there is already a score in that spot or if it is scratched;
    return scores[spot] == 0 ? 1 : -1;
}

//Determins if a hand is a yahtzee
int isYahtzee(int dice[5]) {
    int first = dice[0];
    for (int i = 1; i < 5; ++i) {
        if (dice[i] == first)continue;
        return -1;
    }
    return 1;
}

int isSmallStraight(int dice[5]) {
    //check for 1,2,3,4
    if (doesArrayContainInt(1, dice) + doesArrayContainInt(2, dice) + doesArrayContainInt(3, dice) +
        doesArrayContainInt(4, dice) == 4)
        return 1;
    //check for 2,3,4,5
    if (doesArrayContainInt(2, dice) + doesArrayContainInt(3, dice) + doesArrayContainInt(4, dice) +
        doesArrayContainInt(5, dice) == 4)
        return 1;
    //check for 3,4,5,6
    if (doesArrayContainInt(3, dice) + doesArrayContainInt(4, dice) + doesArrayContainInt(5, dice) +
        doesArrayContainInt(6, dice) == 4)
        return 1;
    return -1;
}

int isLargeStraight(int dice[5]) {
    //check for 1,2,3,4,5
    if (doesArrayContainInt(1, dice) + doesArrayContainInt(2, dice) + doesArrayContainInt(3, dice) +
        doesArrayContainInt(4, dice) + doesArrayContainInt(5, dice) == 5)
        return 1;
    //check for 2,3,4,5,6
    if (doesArrayContainInt(2, dice) + doesArrayContainInt(3, dice) + doesArrayContainInt(4, dice) +
        doesArrayContainInt(5, dice) + doesArrayContainInt(6, dice) == 5)
        return 1;
    return -1;
}

//Check if hand is a fullhouse
int isFullhouse(int dice[5]) {
    int first = dice[0];
    int second = 0;
    for (int i = 1; i < 5; ++i) {
        int new = dice[i];
        if (new == first && second == 0)continue;
        if (new != first && second == 0)second = new;
        if (new != first && new != second)return -1;
    }
    return 1;
}

//Check for three of a kind
int isThreeOfAKind(int dice[5]) {
    for (int i = 0; i < 5; ++i) {
        int die = dice[i];
        int count = 0;
        for (int j = 0; j < 5; ++j) {
            if (dice[j] == die)count++;
        }
        if (count == 3)return 1;
    }
    return -1;
}

//Check for four of a kind
int isFourOfAKind(int dice[5]) {
    for (int i = 0; i < 5; ++i) {
        int die = dice[i];
        int count = 0;
        for (int j = 0; j < 5; ++j) {
            if (dice[j] == die)count++;
        }
        if (count == 4)return 1;
    }
    return -1;
}

//Returns 1 for true -1 for false
int doesArrayContainInt(int value, int array[5]) {
    for (int i = 0; i < 5; ++i) {
        if (array[i] == value)return 1;
    }
    return -1;
}
