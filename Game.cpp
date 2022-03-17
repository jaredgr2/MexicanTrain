#pragma once
#include "stdafx.h"
#include "Game.h"

/* *********************************************************************
Function Name: gameHandler
Purpose: runs the game and displays game's end once done.
Parameters: none
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Game::gameHandler() {
	cout << "Welcome to Mexican Train!" << endl;
	runGame();
	displayGameEnd();
	return;
}
/* *********************************************************************
Function Name: runGame
Purpose: generates Rounds and runs them for duration of game.
Parameters: none
Return Value: none
Algorithm:
	1. If player chooses to load from previous game, generate a round
	from that save and run the round. 
	2. If round returns QUIT, return and end game.
	3. Compute player's round points and scores, display the round end, 
	and check if player wants to continue the game.
	4. If player chooses to not continue, return and end game.
	5. Iterate round number.
	6. Generate new round, run, then steps 2-5. until player chooses to 
	stop.
Assistance Received: none
********************************************************************* */
void Game::runGame() {
	char continueCondition;
	if (askLoadGame()) {
		Round r(roundNumber, humanScore, compScore, l_nextPlayer, l_humanT, l_compT, l_mexT, l_boneY, l_humanDec, l_compDec);
		gameTask = r.runRound();
		if (gameTask == QUIT)return;
		compPoints = r.getCompPoints();
		humanPoints = r.getHumanPoints();
		compScore += compPoints;
		humanScore += humanPoints;
		displayRoundEnd();
		continueCondition = checkContinueGame();
		if (continueCondition == 'n') return;
		roundNumber++;
	}
	do {
		Round r(roundNumber, humanScore, compScore);
		gameTask = r.runRound();
		if (gameTask == QUIT)return;
		compPoints = r.getCompPoints();
		humanPoints = r.getHumanPoints();
		compScore += compPoints;
		humanScore += humanPoints;
		displayRoundEnd();
		continueCondition = checkContinueGame();
		roundNumber++;
	} while (continueCondition != 'n');
	return;
}

/* *********************************************************************
Function Name: checkContinueGame
Purpose: Prompt user if they would like to play another round, validate,
and return choice.
Parameters: none
Return Value: a char, 'y' or 'n' for yes or no
Algorithm:
Assistance Received: none
********************************************************************* */
char Game::checkContinueGame() {
	char playerChoice;
	do {
		cout << "Would you like to play another round (y/n)?";
		cin >> playerChoice;
		playerChoice = tolower(playerChoice);
		if (playerChoice != 'y' && playerChoice != 'n') cout << "\n Invalid choice! Try again.\n";
	} while (playerChoice != 'y' && playerChoice != 'n');
	return playerChoice;
}

/* *********************************************************************
Function Name: displayRoundEnd
Purpose: display player's points and round winner once round ends.
Parameters: none
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Game::displayRoundEnd() {
	if (compPoints > humanPoints) {
		cout << "Human won round with " << humanPoints << " points to computer's " <<compPoints << endl;
	}
	else if (compPoints < humanPoints) {
		cout << "Computer won round with " << compPoints << " points to human's " << humanPoints << endl;
	}
	else {
		cout << "Round ended in draw with human scoring " << humanPoints << " points and computer scoring " << compPoints << endl;
	}
	return;
}

/* *********************************************************************
Function Name: displayGameEnd
Purpose: display player's scores and game winner once game ends.
Parameters: none
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Game::displayGameEnd() {
	if (compScore > humanScore) {
		cout << "Human won game with " << humanScore << " points to computer's " << compScore << endl;
	}
	else if (compScore < humanScore) {
		cout << "Computer won game with " << compScore << " points to human's " << humanScore << endl;
	}
	else {
		cout << "Game ended in draw with human scoring " << humanScore << " points and computer scoring " << compScore << endl;
	}
	return;
}

/* *********************************************************************
Function Name: askLoadGame
Purpose: ask if player would like to load a previous game, and load it if 
so.
Parameters: none
Return Value: bool value - false if player chooses not to load game or 
file can't be opened, and true otherwise.
Algorithm:
Assistance Received: none
********************************************************************* */
bool Game::askLoadGame() {
	char playerChoice;
	string fileName;
	do {
		cout << "Would you like to load a previously saved game (y/n) ?";
		cin >> playerChoice;
		playerChoice = tolower(playerChoice);
		if (playerChoice != 'y' && playerChoice != 'n') cout << "\n Invalid choice! Try again.\n";
	} while (playerChoice != 'y' && playerChoice != 'n');
	if (playerChoice == 'n') return false;
	cout << "Enter the name of the file, minus the extension: ";
	cin >> fileName;
	if (!(loadGame(fileName))) {
		cout << "Could not open file. Returning to main program." << endl;
		return false;
	}
	else return true;
}

/* *********************************************************************
Function Name:loadGame
Purpose: loads previous game state from text file into class members.
Parameters: 
	s, a string which is the filename of the text file the user chooses, 
	minus the .txt extension.
Return Value: bool value, false if could not be opened, true otherwise.
Algorithm:
	1. Open file stream, if cannot be opened, return false.
	2. While end of file flag not reached, get each line and read it 
	into a stringstream.
	3. Depending on the first word read, read the word into a class 
	member.
	4. Set flags depending on whether "Computer" "Human" or "Mexican" were
	read, to determine which tiles to create and push into corresponding
	trains/decks.
	5. Close the game file.
Assistance Received: none
********************************************************************* */
bool Game::loadGame(string s) {
	bool isHuman = false;
	bool isMexican = false;
	ifstream gameState;
	//Open game file
	gameState.open(s + ".txt");
	if(!(gameState)) return false;
	//Read file
	while (!(gameState.eof())) {
	    string line, word;
		getline(gameState, line);
		istringstream lineInput(line);
		while (lineInput >> word) {
			//Read first words of each line
			//Round number
			if (word == "Round:") lineInput >> roundNumber;
			//Next player
			if (word == "Player:")lineInput >> l_nextPlayer;
			//Tells us to read members into human
			if (word == "Human:") isHuman = true;
			//No longer reading human members, train is Mexican
			if (word == "Mexican") {
				isHuman = false;
				isMexican = true;
			}
			//Human player's members
			if (isHuman) {
				//Human score
				if (word == "Score:")lineInput >> humanScore;
				//Human player hand
				if (word == "Hand:") {
					string tile;
					while (lineInput >> tile) {
						Tile* t = new Tile(tile);
						l_humanDec.addToBack(t);
					}
				}
				//Human personal train
				if (word == "Train:") {
					string tile;
					while (lineInput >> tile) {
						Tile* t = new Tile(tile);
						l_humanT.addToBack(t);
					}
				}
			}
			//Mexican train
			else if (isMexican) {
				if (word == "Train:") {
					string tile;
					while (lineInput >> tile) {
						Tile* t = new Tile(tile);
						l_mexT.addToBack(t);
					}
				}
			}
			//Computer's members
			else {
				//Computer score
				if (word == "Score:") lineInput >> compScore;
				//Computer's hand
				if (word == "Hand:") {
					string tile;
					while (lineInput >> tile) {
						Tile* t = new Tile(tile);
						l_compDec.addToBack(t);
					}
				}
				//Computer personal train
				if (word == "Train:") {
					string tile;
					while (lineInput >> tile) {
						Tile* t = new Tile(tile);
						l_compT.addToBack(t);
					}
				}
			}
			//Read in mexican train
			if (word == "Mexican Train:") {
				string tile;
				while (lineInput >> tile) {
					Tile* t = new Tile(tile);
					l_mexT.addToBack(t);
				}
			}
			//Boneyard
			if (word == "Boneyard:") {
				string tile;
				while (lineInput >> tile) {
					Tile* t = new Tile(tile);
					l_boneY.push_back(t);
				}
			}
		}
	}
	gameState.close();
	return true;
}