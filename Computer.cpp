#pragma once
#include "stdafx.h"
#include "Computer.h"

/* *********************************************************************
Function Name: Computer
Purpose: Overriden computer constructor that sets playerHand equal to given
deck.
Parameters:
	d, a Deck which will be copied into computer's playerHand.
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
Computer::Computer(Deck d) {
	playerHand = d;
}

/* *********************************************************************
Function Name: checkEligibility
Purpose: Check which trains and tiles are eligible based on conditions
and player's hand.
Parameters:
	hand, a Deck object passed by reference which is modified by having its
	tiles be set eligible if they are determined to be so.
Return Value: none
Algorithm:
	1. Reset eligibility of trains.
	2. Call determineOrphans on hand and if true, end the function and return.
	3. Check if opponent and personal trains are marked.
	4. Call checkMexEligible on hand.
	5. Check if opponent train is eligible.
	6. If it is, set it to being eligible train and if marker tile is at back,
	check eligible tiles against the second to last tile of train.
	7. Check if personal train is marked and marker tile is at front. If so,
	check eligible tiles against the second tile of train.
	8. Check eligible tiles against front tile of personal train.
	9. If there are still no eligible tiles, personal train is set to
	not being eligible.
	10. If opponent is determined to be eligible, check eligible tiles
	against the back tile of opponent train.
	11. Return.
Assistance Received: none
********************************************************************* */
void Computer::checkEligibility(Deck & hand) {
	resetEligibileTrains();
	if (determineOrphans(hand)) return;
	opponentTrain.checkMarkedTrain();
	personalTrain.checkMarkedTrain();
	checkMexEligible(hand);
	if (opponentTrain.ifMarked()) {
		opponentEligible = true;
		if (opponentTrain.getTileAtBack()->checkMarker()) {
			hand.checkTilesEligible(opponentTrain.getTile(opponentTrain.getMarkedEnd()), "back");
		}
	}
	if (personalTrain.ifMarked() && personalTrain.getTileAtFront()->checkMarker()) {
		hand.checkTilesEligible(personalTrain.getTile(personalTrain.getMarkedBegin()),"front");
	}
	hand.checkTilesEligible(personalTrain.getTileAtFront(),"front");
	if (hand.noEligibleTiles()) {
		personalEligible = false;
	}
	if (opponentEligible) {
		hand.checkTilesEligible(opponentTrain.getTileAtBack(), "back");
	}
	return;
}

/* *********************************************************************
Function Name: determineOrphans
Purpose: Changes the eligibilty of trains depending on if they have orphan
doubles. If any do, check the eligibility of tiles against them.
Parameters:
	hand, a Deck object passed by reference which is modified by having tiles
	be made eligible or not depending on Train eligibility.
Return Value: bool value - false, if no trains have orphan doubles, true
otherwise.
Algorithm:
	1. If none of the trains have orphan doubles, return false.
	2. Make all trains ineligible.
	3. Check each train and if it has an orphan double, make it
	eligible and check the eligibility of player's tiles against
	that train. Return true.
Assistance Received: none
********************************************************************* */
bool Computer::determineOrphans(Deck & hand) {
	if (!(mexicanTrain.ifOrphan() || personalTrain.ifOrphan() || opponentTrain.ifOrphan())) {
		return false;
	}
	personalEligible = false;
	mexEligible = false;
	opponentEligible = false;
	if (mexicanTrain.ifOrphan()) {
		mexEligible = true;
		checkMexEligible(hand);
	}
	if (personalTrain.ifOrphan()) {
		personalEligible = true;
		playerHand.checkTilesEligible(personalTrain.getTileAtFront(), "front");
	}
	if (opponentTrain.ifOrphan()) {
		opponentEligible = true;
		playerHand.checkTilesEligible(opponentTrain.getTileAtBack(), "back");
	}
	return true;
}

/* *********************************************************************
Function Name: displayHelp
Purpose: Displays the output of getHelp() function and then plays tile on
given train using playTileOnTrain().
Parameters:
	chosenTile, a Tile* object passed by getHelp()
	chosenTrain, a string passed by getHelp()
	logic, a string passed by getHelp()
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Computer::displayHelp(Tile* chosenTile, string chosenTrain, string logic) {
	cout << "The computer chose to play ";
	chosenTile->print(cout);
	cout << "on ";
	if (chosenTrain == "Mexican") cout << "Mexican ";
	if (chosenTrain == "opponent") cout << "opponent's ";
	if (chosenTrain == "personal") cout << "personal ";
	cout << "train because ";
	chosenTile->print(cout);
	if (logic == "double") cout << "is double ";
	if (logic == "large") cout << "is largest eligible tile in hand ";
	if (chosenTrain == "opponent") cout << "and opponent's train had marker or orphan double.";
	cout << endl;
	playTileOnTrain(chosenTrain, chosenTile);
	return;
}

/* *********************************************************************
Function Name: handleNoEligibles
Purpose: Handles if computer has no eligible tiles - attempts to draw 
from boneyard and play tile if eligible.
Parameters: none
Return Value: none
Algorithm:
	1. Tell user computer is attempting to draw from boneyard.
	2. If boneyard is empty, place marker if marker is not already placed,
	and end computer turn. (Output changes depending on if marker is already
	there.)
	3. Add front tile of boneyard to playerHand. Remove this tile from
	boneyard and shrink size of boneyard to fit.
	4. Check the eligibilty of computer's tiles again. If there are still
	no eligible tiles, place marker if marker is not already placed,
	and end turn. (Output changes depending on if marker is already
	there.)
	5. If tile is eligible, call getHelp() once more, reset eligibility
	pf tiles and return.
Assistance Received: none
********************************************************************* */
void Computer::handleNoEligibles() {
	cout << "Computer has no eligible tiles! It will attempt to draw from boneyard." << endl;
	if (boneYard.empty()) {
		if (personalTrain.ifMarked()) {
			cout << "Boneyard empty! Ending computer's turn." << endl;
			return;
		}
		cout << "Boneyard empty! Computer places marker and ends turn." << endl;
		playMarker();
		return;
	}
	playerHand.addToBack(boneYard.front());
	boneYard.pop_front();
	boneYard.shrink_to_fit();
	checkEligibility(playerHand);
	if (playerHand.noEligibleTiles()) {
		if (personalTrain.ifMarked()) {
			cout << "Tile drawn not playable. Ending computer's turn." << endl;
			return;
		}
		cout << "Tile drawn not playable. Computer places marker and ends turn." << endl;
		playMarker();
		return;
	}
	cout << "Tile is playable. Proceeding." << endl;
	getHelp(playerHand, firstTurn);
	playerHand.resetEligibility();
	return;
}

/* *********************************************************************
Function Name: personalEligibles
Purpose: Checks if a deck has tiles that are eligible for personal train,
then returns the Deck with only those tiles.
Parameters:
	personal, Deck object whose tiles are set eligible or not, and then
	removed if ineligible.
Return Value: modified Deck with no ineligible tiles.
Algorithm:
Assistance Received: none
********************************************************************* */
Deck Computer::personalEligibles(Deck personal) {
	if (personalTrain.ifMarked() && personalTrain.getTileAtFront()->checkMarker()) {
		personal.checkTilesEligible(personalTrain.getTile(personalTrain.getMarkedBegin()), "front");
	}
	personal.checkTilesEligible(personalTrain.getTileAtFront(), "front");
	personal.removeIneligibles();
	return personal;
}

/* *********************************************************************
Function Name: opponentEligibles
Purpose: Checks if a deck has tiles that are eligible for opponent train,
then returns the Deck with only those tiles.
Parameters:
	opponent, Deck object whose tiles are set eligible or not, and then
	removed if ineligible.
Return Value: modified Deck with no ineligible tiles.
Algorithm:
Assistance Received: none
********************************************************************* */
Deck Computer::opponentEligibles(Deck opponent) {
	if (opponentTrain.ifMarked() && opponentTrain.getTileAtBack()->checkMarker()) {
		opponent.checkTilesEligible(opponentTrain.getTile((opponentTrain.getMarkedEnd())), "back");
	}
	opponent.checkTilesEligible(opponentTrain.getTileAtBack(), "back");
	opponent.removeIneligibles();
	return opponent;
}

/* *********************************************************************
Function Name: checkMexEligible
Purpose: Checks various cases to see if mexican Train is eligible to be
played on and which tiles are eligible if so.
Parameters:
	hand, a Deck object passed by reference which is modified by having its
	tiles be set eligible if they are determined to be so.
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Computer::checkMexEligible(Deck & hand) {
	//When empty, check against the engine
	if (mexicanTrain.ifEmpty()) {
		hand.checkTilesEligible(opponentTrain.getTileAtFront(), "back");
	}
	else {
		hand.checkTilesEligible(mexicanTrain.getTileAtBack(), "back");
	}
	if (hand.noEligibleTiles()) {
		mexEligible = false;
		return;
	}
	return;
}

/* *********************************************************************
Function Name: playMarker
Purpose: Creates new marker tile, places it on personal train and sets
marker flag on personal train.
Parameters: none
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Computer::playMarker() {
	Tile* marker = new Tile('m');
	personalTrain.addToFront(marker);
	personalTrain.checkMarkedTrain();
	return;
}

/* *********************************************************************
Function Name: playTurn
Purpose: Plays the computer's turn.
Parameters:
	turn, unsigned short representing number of tiles currently played
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Computer::playTurn(unsigned short turn) {
	getHelp(playerHand, turn);
	playerHand.resetEligibility();
	return;
}

/* *********************************************************************
Function Name: playTileOnTrain
Purpose: Plays a given tile on a given train.
Parameters:
	chosenTrain, a string
	chosenTile, a Tile* object
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Computer::playTileOnTrain(string chosenTrain, Tile* chosenTile) {
	if (chosenTrain == "personal") {
		personalTrain.addToFront(chosenTile);
		if (personalTrain.ifMarked()) {
			personalTrain.unmarkTrain();
		}
		playerHand.removeFromDeck(chosenTile);
	}
	if (chosenTrain == "opponent") {
		opponentTrain.addToBack(chosenTile);
		playerHand.removeFromDeck(chosenTile);
	}
	if (chosenTrain == "Mexican") {
		if (mexicanTrain.ifEmpty())chosenTile->swapPips();
		mexicanTrain.addToBack(chosenTile);
		playerHand.removeFromDeck(chosenTile);
	}
	return;
}

/* *********************************************************************
Function Name: playerMenu
Purpose: Print menu of player decisions before a turn, have player make a
choice and validate.
Parameters: none
Return Value:
	unsigned short representing the choice player has made on menu.
Algorithm:
Assistance Received: none
********************************************************************* */
unsigned short Computer::playerMenu() {
	unsigned short playerChoice;
	do {
		cout << "Menu:" << endl;
		cout << "1. Save the game" << endl;
		cout << "2. Make a move" << endl;
		cout << "3. Quit" << endl;
		cin >> playerChoice;
		if (playerChoice > QUIT || playerChoice < SAVE)cout << "Invalid menu option. Select a valid option.\n";
	} while (playerChoice > QUIT || playerChoice < SAVE);
	if (playerChoice == QUIT) playerChoice++;
	return playerChoice;
}