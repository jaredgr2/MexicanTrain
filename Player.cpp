#pragma once
#include "stdafx.h"
#include "Player.h"

/* *********************************************************************
Function Name: addToDeck
Purpose: add given tile to back of player hand.
Parameters: 
	Tile* t, pushed to back of tileDeck.
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Player::addToDeck(Tile* t) {
	playerHand.addToBack(t);
}

/* *********************************************************************
Function Name: removeFromDeck
Purpose: remove given tile from player hand.
Parameters:
	Tile* t, removed from tileDeck.
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Player::removeFromDeck(Tile* t) {
	playerHand.removeFromDeck(t);
}

/* *********************************************************************
Function Name: conditionsSecondDouble
Purpose: check if second double can be played given the tiles that will
remain once that tile is played.
Parameters: none
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Player::conditionsSecondDouble(Tile* t, Deck& hand) {
	hand.removeFromDeck(t);
	if (hand.ifEmpty()) {
		hand.addToBack(t);
		return;
	}
	else {
		hand.resetEligibility();
		checkEligibility(hand);
		if (!(hand.noEligibleTiles())) {
			hand.addToBack(t);
			return;
		}
	}
	t->setIneligible();
	hand.addToBack(t);
	return;
}

/* *********************************************************************
Function Name: resetEligibleTrains
Purpose: resets eligible train flags to most common starting ones.
Parameters: none
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Player::resetEligibileTrains() {
	personalEligible = true;
	mexEligible = true;
	opponentEligible = false;
}

/* *********************************************************************
Function Name: destroyHandPointers
Purpose: destroys dynamic memory allocated to playerHand
Parameters: none
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Player::destroyHandPointers() {
	playerHand.destroyPointers();
}

/* *********************************************************************
Function Name: getHelp
Purpose: determines which tile should be played on what train given
a player's hand and the amount of tiles it has already played.
Parameters: 
	cloneHand, deck that is usually passed as playerHand
	tilesPlayed, unsigned short keeping track of how many tiles
	have been played.
Return Value: none
Algorithm:
	1. If player's hand is empty, return.
	2. Reset eligible tiles in hand and eligible trains.
	3. Check eligibility of hand's tiles. If not eligible tiles,
	call handleNoEligibles() and return.
	4. Create temporary Tile* and string to store chosen train.
	5. Create 3 Decks for the eligible tiles of each train.
	6. Reset hand's eligibility once more and check Mexican eligible
	tiles for hand.
	7. Store tiles in mexTiles, remove ineligibles.
	8. Repeat steps 5 and 6, but for personal and opponent trains.
	9. Call determineOrphans() to find if there are any orphan
	doubles on trains and if so, which ones are eligible.
	If true, proceed.
	10. If false, set eligibilty of train to false for each train
	that has an empty eligible tiles Deck. If opponent eligible
	tile deck is not empty, check if opponent train is marked.
	If it is, set opponent train to eligible.
	11.Set all tiles in each deck to eligible.
	12. Check if each deck has a double within it.
	13. If opponent deck has doubles, choose biggest double
	and call getHelp() with logic of "double".
	14. If Mexican deck has doubles, choose biggest double
	and call getHelp() with logic of "double"
	15. If personal deck has doubles, choose biggest double
	and call getHelp() with logic of "double"
	16. If no decks have doubles, choose largest tile out of 
	all eligible decks and call getHelp() with logic of 
	"large"
Assistance Received: none
********************************************************************* */
void Player::getHelp(Deck cloneHand, unsigned short tilesPlayed) {
	//Empty hand - nothing to be played
	if (cloneHand.ifEmpty())return;
	cloneHand.resetEligibility();
	resetEligibileTrains();
	checkEligibility(cloneHand);
	//No eligible tiles - handle this
	if (cloneHand.noEligibleTiles()) {
		handleNoEligibles();
		return;
	}
	Tile* chosenTile;
	string chosenTrain;
	cloneHand.resetEligibility();
	//Find Mexican eligibles
	checkMexEligible(cloneHand);
	Deck mexTiles(cloneHand);
	mexTiles.removeIneligibles();
	cloneHand.resetEligibility();
	//Find personal eligibles
	Deck personalTiles = personalEligibles(cloneHand);
	cloneHand.resetEligibility();
	//Find opponent eligibles
	Deck opponentTiles = opponentEligibles(cloneHand);
	//Find out if there are any orphan doubles
	bool orphanTrue = determineOrphans(cloneHand);
	//If no orphan doubles, determine eligible trains
	if (!(orphanTrue)) {
		if (mexTiles.ifEmpty()) mexEligible = false;
		if (personalTiles.ifEmpty()) personalEligible = false;
		if (opponentTiles.ifEmpty()) opponentEligible = false;
		else if (opponentTrain.ifMarked())opponentEligible = true;
	}
	//Display message if there are orphan doubles
	else {
		cout << "Only the following plays can be made due to orphan double!" << endl;
	}
	//Set all tiles to eligible. Since tiles are pointers,
	//they might be changed to ineligible by the Deck they are in.
	//For lifetime of function we will make them eligible.
	personalTiles.setAllEligible();
	opponentTiles.setAllEligible();
	mexTiles.setAllEligible();
	//Check for doubles in each deck of eligibles.
	bool personalHasDouble = personalTiles.checkForDoubles();
	bool mexHasDouble = mexTiles.checkForDoubles();
	bool oppHasDouble = opponentTiles.checkForDoubles();

	if (opponentEligible && oppHasDouble) {
	   chosenTile = opponentTiles.findLargestDouble();
	   //Check if a second double can be played
	   if (tilesPlayed == 1) {
		   conditionsSecondDouble(chosenTile, cloneHand);
	   }
	   if (chosenTile->checkEligible()) {
		   chosenTrain = "opponent";
		   displayHelp(chosenTile, chosenTrain, "double");
		   cloneHand.removeFromDeck(chosenTile);
		   getHelp(cloneHand, tilesPlayed + 1);
	   }
	}
	else if (mexEligible && mexHasDouble) {
	   chosenTile = mexTiles.findLargestDouble();
	   //Check if a second double can be played
	   if (tilesPlayed == 1) {
		   conditionsSecondDouble(chosenTile, cloneHand);
	   }
	   if (chosenTile->checkEligible()) {
		   chosenTrain = "Mexican";
		   displayHelp(chosenTile, chosenTrain, "double");
		   cloneHand.removeFromDeck(chosenTile);
		   getHelp(cloneHand, tilesPlayed + 1);
	   }
	}
	else if (personalEligible && personalHasDouble) {
		chosenTile = personalTiles.findLargestDouble();
		//Check if a second double can be played
		if (tilesPlayed == 1) {
			conditionsSecondDouble(chosenTile, cloneHand);
		}
		if (chosenTile->checkEligible()) {
			chosenTrain = "personal";
			displayHelp(chosenTile, chosenTrain, "double");
			cloneHand.removeFromDeck(chosenTile);
			getHelp(cloneHand, tilesPlayed + 1);
		}
	}
	//No doubles case
	else {
		//All trains eligible
		if (personalEligible && opponentEligible && mexEligible) {
			Tile* personalBiggest = personalTiles.findLargestTile();
			Tile* mexBiggest = mexTiles.findLargestTile();
			Tile* oppBiggest = opponentTiles.findLargestTile();
			//Opponent has largest tile to be played
			if ((oppBiggest->calculateVal() >= mexBiggest->calculateVal()) && (oppBiggest->calculateVal() >= personalBiggest->calculateVal())) {
				chosenTile = oppBiggest;
				chosenTrain = "opponent";
				displayHelp(chosenTile, chosenTrain, "large");
			}
			//Mexican has largest tile to be played
			else if ((mexBiggest->calculateVal() >= personalBiggest->calculateVal()) && (mexBiggest->calculateVal() > oppBiggest->calculateVal())) {
				chosenTile = mexBiggest;
				chosenTrain = "Mexican";
				displayHelp(chosenTile, chosenTrain, "large");
			}
			//Personal has largest tile to be played
			else {
				chosenTile = personalBiggest;
				chosenTrain = "personal";
				displayHelp(chosenTile, chosenTrain, "large");
			}
		}
		//Personal isn't eligible
		else if (!(personalEligible)) {
			//Opponent and Mexican are only eligible
			if (opponentEligible && mexEligible) {
				Tile* mexBiggest = mexTiles.findLargestTile();
				Tile* oppBiggest = opponentTiles.findLargestTile();
				//Opponent has largest tile to be played
				if (oppBiggest->calculateVal() >= mexBiggest->calculateVal()) {
					chosenTile = oppBiggest;
					chosenTrain = "opponent";
					displayHelp(chosenTile, chosenTrain, "large");
				}
				//Mexican has largest tile to be played
				else {
					chosenTile = mexBiggest;
					chosenTrain = "Mexican";
					displayHelp(chosenTile, chosenTrain, "large");
				}
			}
			else{
				//Mexican can only be played on
				if (!(opponentEligible)) {
					chosenTile = mexTiles.findLargestTile();
					chosenTrain = "Mexican";
					displayHelp(chosenTile, chosenTrain, "large");
				}
				//Opponent can only be played on
				if (!(mexEligible)) {
					chosenTile = opponentTiles.findLargestTile();
					chosenTrain = "opponent";
					displayHelp(chosenTile, chosenTrain, "large");
				}
			}
		}
		else if(personalEligible) {
			//Personal and mexican are only eligible
			if (mexEligible && !(opponentEligible)) {
				Tile* mexBiggest = mexTiles.findLargestTile();
				Tile* personalBiggest = personalTiles.findLargestTile();
				//Mexican has largest tile to be played
				if (mexBiggest->calculateVal() >= personalBiggest->calculateVal()) {
					chosenTile = mexBiggest;
					chosenTrain = "Mexican";
					displayHelp(chosenTile, chosenTrain, "large");
				}
				//Personal has largest tile to be played
				else {
					chosenTile = personalBiggest;
					chosenTrain = "personal";
					displayHelp(chosenTile, chosenTrain, "large");
				}
			}
			//Personal and opponent are only eligible
			else if (opponentEligible && !(mexEligible)) {
				Tile* oppBiggest = opponentTiles.findLargestTile();
				Tile* personalBiggest = personalTiles.findLargestTile();
				//Opponent has largest tile to be played
				if (oppBiggest->calculateVal() >= personalBiggest->calculateVal()) {
					chosenTile = oppBiggest;
					chosenTrain = "opponent";
					displayHelp(chosenTile, chosenTrain, "large");
				}
				//Personal has largest tile to be played
				else {
					chosenTile = personalBiggest;
					chosenTrain = "personal";
					displayHelp(chosenTile, chosenTrain, "large");
				}
			}
			//Personal can only be played on
			else {
				chosenTile = personalTiles.findLargestTile();
				chosenTrain = "personal";
				displayHelp(chosenTile, chosenTrain, "large");
			}
		}
	}
	return;
} 

/* *********************************************************************
Function Name: loadTrains
Purpose: load Trains and boneYard into Player object.
Parameters: 
	personal, Train to be loaded into personalTrain
	opponent, Train to be loaded into opponentTrain
	mexican, Train to be loaded into mexicanTrain
	bone, deque<Tile*> to be loaded into boneYard
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Player::loadTrains(Train personal, Train opponent, Train mexican, deque<Tile*> bone) {
	personalTrain = personal;
	opponentTrain = opponent;
	mexicanTrain = mexican;
	boneYard = bone;
	return;
}
