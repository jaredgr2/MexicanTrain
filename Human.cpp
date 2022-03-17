#pragma once
#include "stdafx.h"
#include "Human.h"

/* *********************************************************************
Function Name: Human
Purpose: Overriden human constructor that sets playerHand equal to given
deck.
Parameters:
	d, a Deck which will be copied into human's playerHand.
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
Human::Human(Deck d) {
	playerHand = d;
}

/* *********************************************************************
Function Name: playTile
Purpose: Prompt user for tile and validate their input, then return the 
tile.
Parameters: none
Return Value: a Tile* in player's hand
Algorithm:
Assistance Received: none
********************************************************************* */
Tile* Human::playTile() {
	deque<Tile*>::const_iterator it;
	string chosenTile;
	do {
		cout << "Which tile will you play? ";
		cin >> chosenTile;
		it = playerHand.findTile(chosenTile);
		if (it >= playerHand.getEnd()) cout << "\n Invalid selection! Please select a tile that is in your hand. \n";
	} while (it >= playerHand.getEnd());
	cout << endl;
	return playerHand.getTile(it);
}

/* *********************************************************************
Function Name: verifyTrain
Purpose: Makes sure that selected tile is played on its eligible train.
Parameters:
	t, the Tile* which has been chosen and is checked against all 3 trains.
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
string Human::verifyTrain(Tile* t) {
	string chosenTrain;
	bool wrongTrain;
	do {
		wrongTrain = false;
		chosenTrain = selectEligibleTrain();
		if (chosenTrain == "personal" && !(personalTrain.checkTileMatchBack(t))) {
			wrongTrain = true;
		}
		if (chosenTrain == "mexican") {
			if (mexicanTrain.ifEmpty()) {
				if (!(personalTrain.checkTileMatchFront(t))) wrongTrain = true;
			}
			else {
				if (!(mexicanTrain.checkTileMatchBack(t))) wrongTrain = true;
			}
		}
		if (chosenTrain == "opponent" && !(opponentTrain.checkTileMatchFront(t))) {
			wrongTrain = true;
		}
		if (wrongTrain) cout << "Tile can't be played on that train. Choose a different one." << endl;
	} while (wrongTrain);
	return chosenTrain;
}

/* *********************************************************************
Function Name: selectEligibleTrain
Purpose: Makes sure that selected train is eligible to be played on.
Parameters: none
Return Value: string chosenTrain, which is the train selected by player.
Algorithm:
Assistance Received: none
********************************************************************* */
string Human::selectEligibleTrain() {
	string chosenTrain;
	bool ineligibleTrain;
	do {
		ineligibleTrain = false;
		chosenTrain = selectTrain();
		if (chosenTrain == "personal" && (!(personalEligible))) {
			ineligibleTrain = true;
		}
		else if (chosenTrain == "opponent" && (!(opponentEligible))) {
			ineligibleTrain = true;
		}
		else if (chosenTrain == "mexican" && (!(mexEligible))) {
			ineligibleTrain = true;
		}
		if (ineligibleTrain) cout << "Train is ineligible. Please select different train." << endl;
	} while (ineligibleTrain);
	return chosenTrain;
}

/* *********************************************************************
Function Name: selectTrain
Purpose: Makes sure that selected train is one of the three possible trains.
Parameters: none
Return Value: string chosenTrain, the train player has selected.
Algorithm:
Assistance Received: none
********************************************************************* */
string Human::selectTrain() {
	string chosenTrain;
	do {
		cout << "Which train will you play on? Personal, Mexican, or Opponent?";
		cin >> chosenTrain;
		for (auto it : chosenTrain)it = tolower(it);
	} while ((chosenTrain != "personal") && (chosenTrain != "mexican") && (chosenTrain != "opponent"));
	cout << endl;
	return chosenTrain;
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
Deck Human::personalEligibles(Deck personal) {
	if (personalTrain.ifMarked() && personalTrain.getTileAtBack()->checkMarker()) {
		personal.checkTilesEligible(personalTrain.getTile(personalTrain.getMarkedEnd()), "back");
	}
	personal.checkTilesEligible(personalTrain.getTileAtBack(), "back");
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
Deck Human::opponentEligibles(Deck opponent) {
	if (opponentTrain.ifMarked() && opponentTrain.getTileAtFront()->checkMarker()) {
		opponent.checkTilesEligible(opponentTrain.getTile(opponentTrain.getMarkedBegin()), "front");
	}
	opponent.checkTilesEligible(opponentTrain.getTileAtFront(), "front");
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
void Human::checkMexEligible(Deck & hand) {
	//When empty, check against the engine
	if (mexicanTrain.ifEmpty()) {
		hand.checkTilesEligible(personalTrain.getTileAtFront(), "back");
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
Function Name: displayHelp
Purpose: Displays the output of getHelp() function.
Parameters:
	chosenTile, a Tile* object passed by getHelp()
	chosenTrain, a string passed by getHelp()
	logic, a string passed by getHelp()
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Human::displayHelp(Tile* chosenTile, string chosenTrain, string logic) {
	cout << "I suggest you play ";
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
	return;
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
	6. If it is, set it to being eligible train and if marker tile is at front,
	check eligible tiles against the second tile of train.
	7. Check if personal train is marked and marker tile is at back. If so,
	check eligible tiles against the second to last tile of train.
	8. Check eligible tiles against back tile of personal train.
	9. If there are still no eligible tiles, personal train is set to
	not being eligible.
	10. If opponent is determined to be eligible, check eligible tiles
	against the front tile of opponent train.
	11. Return.
Assistance Received: none
********************************************************************* */
void Human::checkEligibility(Deck& hand) {
	resetEligibileTrains();
	if (determineOrphans(hand)) return;
	opponentTrain.checkMarkedTrain();
	personalTrain.checkMarkedTrain();
	checkMexEligible(hand);
	if (opponentTrain.ifMarked()) {
		opponentEligible = true;
		if (opponentTrain.getTileAtFront()->checkMarker()) {
			playerHand.checkTilesEligible(opponentTrain.getTile(opponentTrain.getMarkedBegin()), "front");
		}
	}
	if (personalTrain.ifMarked() && personalTrain.getTileAtBack()->checkMarker()) {
		playerHand.checkTilesEligible(personalTrain.getTile(personalTrain.getMarkedEnd()), "back");
	}
	playerHand.checkTilesEligible(personalTrain.getTileAtBack(), "back");
	if (playerHand.noEligibleTiles()) {
		personalEligible = false;
	}
	if (opponentEligible) {
		playerHand.checkTilesEligible(opponentTrain.getTileAtFront(), "front");
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
void Human::playMarker() {
	Tile* marker = new Tile('m');
	personalTrain.addToBack(marker);
	personalTrain.checkMarkedTrain();
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
void Human::playTileOnTrain(string chosenTrain, Tile* chosenTile) {
	if (chosenTrain == "personal") {
		personalTrain.addToBack(chosenTile);
		if (personalTrain.ifMarked()) {
			personalTrain.unmarkTrain();
		}
		playerHand.removeFromDeck(chosenTile);
	}
	if (chosenTrain == "opponent") {
		opponentTrain.addToFront(chosenTile);
		playerHand.removeFromDeck(chosenTile);
	}
	if (chosenTrain == "mexican") {
		if (mexicanTrain.ifEmpty())chosenTile->swapPips();
		mexicanTrain.addToBack(chosenTile);
		playerHand.removeFromDeck(chosenTile);
	}
	return;
}

/* *********************************************************************
Function Name: playTurn
Purpose: Function for player to make move which is called recursively
if more than 1 tile can be played.
Parameters:
	numTilesPlayed, unsigned short which tracks how many tiles have been
	played depending on the function call.
Return Value: none
Algorithm:
	1. If player hand is empty, end player's turn.
	2. Otherwise, reset eligible tiles and check eligiblity of current
	tiles.
	3. If no tiles are eligible, call boneYardDraw(), if that returns false,
	end the player's turn.
	4. If tiles are eligible, prompt user for tile using playTile() and 
	validate.
	5. Prompt user for train using verifyTrain() with user's tile, 
	validate this input.
	6. If the tile wasn't a double, reset eligible tiles and end turn.
	7. If tile was a double, recursively call playTurn() with 
	numTilesPlayed incremented by one.
Assistance Received: none
********************************************************************* */
void Human::playTurn(unsigned short numTilesPlayed) {
	if (playerHand.ifEmpty())return;
	playerHand.resetEligibility();
	checkEligibility(playerHand);
	if (playerHand.noEligibleTiles()) {
		if (!(boneYardDraw())) {
			playerHand.resetEligibility();
			return;
		}
	}
	Tile* tilePlayed;
	do {
		checkEligibility(playerHand);
		tilePlayed = playTile();
		//Check if another double can be played if a double has already been played during this turn.
		if (numTilesPlayed == 1 && tilePlayed->checkDouble())conditionsSecondDouble(tilePlayed, playerHand);
		if (!(tilePlayed->checkEligible())) {
			cout << "Tile is not eligible. Play a different tile." << endl;
			playerHand.resetEligibility();
		}
	} while (!(tilePlayed->checkEligible()));
	checkEligibility(playerHand);
	string chooseTrain = verifyTrain(tilePlayed);
	playTileOnTrain(chooseTrain, tilePlayed);
	if (!(tilePlayed->checkDouble())) {
		playerHand.resetEligibility();
		return;
	}
	playerHand.resetEligibility();
	cout << "Double played! Play another double or non-double eligible tile." << endl;
	playTurn(numTilesPlayed + 1);
	return;
}

/* *********************************************************************
Function Name: boneYardDraw
Purpose: Attempt to draw a tile from boneyard and have user play it.
Parameters: none
Return Value: bool value - true if eligible tile found, false otherwise.
Algorithm:
	1. Tell user we are attempting to draw from boneyard.
	2. If boneyard is empty, place marker if marker is not already placed,
	and return false. (Output changes depending on if marker is already 
	there.)
	3. Add front tile of boneyard to playerHand. Remove this tile from 
	boneyard and shrink size of boneyard to fit.
	4. Check the eligibilty of player's tiles again. If there are still
	no eligible tiles, place marker if marker is not already placed, 
	and return false. (Output changes depending on if marker is already 
	there.)
	5. If tile is eligible, print the tile using a temp tile object and
	reset eligible tiles. Tell the player they must play this tile. Return.
Assistance Received: none
********************************************************************* */
bool Human::boneYardDraw() {
	cout << "No eligible tiles! Attempting to draw from boneyard.." << endl;
	if (boneYard.empty()) {
		if (personalTrain.ifMarked()) {
			cout << "Boneyard empty! Ending turn." << endl;
			return false;
		}
		cout << "Boneyard empty! Placing marker and ending turn." << endl;
		playMarker();
		return false;
	}
	playerHand.addToBack(boneYard.front());
	Tile* temp = boneYard.front();
	boneYard.pop_front();
	boneYard.shrink_to_fit();
	checkEligibility(playerHand);
	if (playerHand.noEligibleTiles()) {
		if (personalTrain.ifMarked()) {
			cout << "Tile drawn not playable. Ending turn." << endl;
			return false;
		}
		cout << "Tile drawn not playable. Placing marker and ending turn." << endl;
		playMarker();
		return false;
	}
	cout << "Tile drawn successfully. You must play this tile. - >";
	temp->print(cout);
	cout << endl;
	playerHand.resetEligibility();
	return true;
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
bool Human::determineOrphans(Deck& hand) {
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
		playerHand.checkTilesEligible(personalTrain.getTileAtBack(), "back");
	}
	if (opponentTrain.ifOrphan()) {
		opponentEligible = true;
		playerHand.checkTilesEligible(opponentTrain.getTileAtFront(), "front");
	}
	return true;
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
unsigned short Human::playerMenu() {
	unsigned short playerChoice;
	do {
		cout << "Menu:" << endl;
		cout << "1. Save the game" << endl;
		cout << "2. Make a move" << endl;
		cout << "3. Ask for help" << endl;
		cout << "4. Quit the game" << endl;
		cin >> playerChoice;
		if (playerChoice > QUIT || playerChoice < SAVE)cout << "Invalid menu option. Select a valid option.\n";
	} while (playerChoice > QUIT || playerChoice < SAVE);
	return playerChoice;
}