#pragma once
#include "stdafx.h"
#include "Deck.h"

/* *********************************************************************
Function Name: Deck
Purpose: Deck copy constructor
Parameters: 
	Deck d, a Deck object passed by const reference. It is not changed.
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
Deck::Deck(const Deck& d) {
	tileDeck = d.tileDeck;
}

/* *********************************************************************
Function Name:~Deck
Purpose: Deck destructor - clears tileDeck
Parameters: none
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
Deck::~Deck() {
	tileDeck.clear();
}

/* *********************************************************************
Function Name: addToBack
Purpose: Adds tile to back of tileDeck
Parameters: 
	Tile* t, pushed to back of tileDeck.
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Deck::addToBack(Tile* t) {
	tileDeck.push_back(t);
}

/* *********************************************************************
Function Name: displayDeck
Purpose: Prints entire deck to ostream
Parameters: 
	os, an ostream passed by reference. Changed by results of print() for each 
	tile iterated over being added to ostream.
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Deck::displayDeck(ostream & os) {
	for (auto it : tileDeck) {
		it->print(os);
	}
	return;
}

/* *********************************************************************
Function Name: displayDeck2
Purpose:Prints entire deck to ostream
Parameters: 
	os, an ostream passed by reference. Changed by results of print() for each 
	tile iterated over being added to ostream.
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Deck::displayDeck2(ostream& os) {
	deque<Tile*>::iterator it = tileDeck.begin() + 1;
	while (it != tileDeck.end()) {
		(*it)->print(os);
		++it;
	}
}

/* *********************************************************************
Function Name: shuffleDeck
Purpose: Shuffles tileDeck elements
Parameters: none
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Deck::shuffleDeck() {
	//Random engine object that we will use to shuffle the tileDeck queue
	default_random_engine rando(random_device{}());
	shuffle(tileDeck.begin(), tileDeck.end(), rando);
	return;
}

/* *********************************************************************
Function Name: emptyDeck
Purpose: Empties tileDeck.
Parameters: none
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Deck::emptyDeck() {
	for (auto it : tileDeck) {
		removeFromDeck(it);
	}
}

/* *********************************************************************
Function Name: findTile
Purpose: Finds the location of tile in deck given its string representation
Parameters: 
	string s, which represents display form of tile to be found.
Return Value: 
	const iterator to found tile's location or if not found, end iterator
Algorithm:
	1.Check if string s matches regex
	2.If it doesn't, check if it is "m"
	3.If "m" iterate through tileDeck to find marker and return iterator to that tile
	4.If not "m" return iterator to end of tileDeck
	5.If s matches regex, iterate and find tile that matches pips
	6.If tile matching is found, return iterator to location
	7.Otherwise return end iterator
Assistance Received: none
********************************************************************* */
deque<Tile*>::const_iterator Deck::findTile(string s) {
	deque<Tile*>::const_iterator it;
	//Regular expression to validate format of string being passed.
	//We will only accept a digit followed by dash, followed by digit.
	regex tileFormat("\\d-\\d");
	//If the string doesn't match the regex, we check if it is a marker.
	if (!(regex_match(s, tileFormat))) {
		//If it is a marker, we iterate through the tileDeck to find the tile where
		//markerTile flag is true. There will only ever be one.
		if (s == "m") {
			for (it = tileDeck.begin(); it != tileDeck.end(); it++) {
				if ((*it)->checkMarker()) {
					return it;
				}
			}
		}
		//Return end since tile could not be found matching string
		else {
			return getEnd();
		}
	}
	//Get string equivalents to each pip then convert to unsigned shorts
	string pip1str = s.substr(0,1);
	string pip2str = s.substr(2);
	unsigned short pip1 = (unsigned short) stoul(pip1str,nullptr,10);
	unsigned short pip2 = (unsigned short) stoul(pip2str,nullptr,10);
	for (it = tileDeck.begin(); it != tileDeck.end(); it++) {
		//If a tile's pips match the pips we got from the string, return that tile.
		if ((pip1 == (*it)->getPip1()) && (pip2 == (*it)->getPip2())) {
			return it;
		}
	}
	//Return end if tile could not be found
	return getEnd();
}
/* *********************************************************************
Function Name: getTile
Purpose: Return tile at given location
Parameters: 
	it, const iterator to Tile* 
Return Value: the Tile* object at location it
Algorithm:
Assistance Received: none
********************************************************************* */
Tile* Deck::getTile(deque<Tile*>::const_iterator it) const {
	return *it;
}

/* *********************************************************************
Function Name: findLargestTile
Purpose: Find and return tile with largest value 
Parameters: none
Return Value: Tile* with largest value in deque
Algorithm:
Assistance Received: none
********************************************************************* */
Tile* Deck::findLargestTile() const {
	//Use front tile of deque - if no larger tile is found, this one is the largest.
	Tile* t = tileDeck.front();
	for (auto it : tileDeck) {
		//If the sum of pips of one tile is bigger than t, set t to that tile.
		if (it->calculateVal() > t->calculateVal())
			t = it;
	}
	return t;
}

/* *********************************************************************
Function Name: findLargestDouble
Purpose: Find and return double tile with largest value 
Parameters: none
Return Value: double Tile* with largest value in deque
Algorithm:
Assistance Received: none
********************************************************************* */
Tile* Deck::findLargestDouble()const {
	//0,0 is the smallest double that can be returned from this.
	Tile* t = new Tile(0, 0);
	for (auto it : tileDeck) {
		//If the tile is a double and sum of pips is larger, set t to that tile.
		if (it->checkDouble() && it->calculateVal() > t->calculateVal())
			t = it;
	}
	return t;
}

/* *********************************************************************
Function Name: removeFromDeck
Purpose: Remove given tile from Deck
Parameters: 
	t, a Tile* within the deck.
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Deck::removeFromDeck(Tile* t) {
	deque<Tile*>::const_iterator it;
	//Find tile based on whether it is marker or not.
	if (t->checkMarker()) {
		it = findTile("m");
	}
	else {
		//If not marker, get string format of tile and use this to find the tile.
		string tileFormat = to_string(t->getPip1()) + "-" + to_string(t->getPip2());
		it = findTile(tileFormat);
	}
	//Delete tile and shrink deck's size to fit.
	tileDeck.erase(it);
	tileDeck.shrink_to_fit();
	return;
}

/* *********************************************************************
Function Name: checkTilesEligible
Purpose: Check tiles in a deck against a given tile, depending on which end.
Parameters: 
	t, a Tile* that is compared to
	end, a string that determines which pip will be compared to
Return Value: none
Algorithm:
	1.Iterate through tileDeck
	2.If back end matching, check if first pip is equal to t's second pip
	3.If it is, set eligible flag on tile
	4.Swap pips on tile, then repeat steps 2 and 3
	5.Swap pips again
	6.If front end matching, check if second pip is equal to t's first
	7.See steps 3-5

Assistance Received: none
********************************************************************* */
void Deck::checkTilesEligible(Tile* t, string end) {
	for (auto it : tileDeck) {
		//Back-end matching
		if (end == "back") {
			if (it->getPip1() == t->getPip2()) {
				it->setEligible();
			}
			it->swapPips();
			if (it->getPip1() == t->getPip2()) {
				it->setEligible();
			}
			it->swapPips();
		}
		//Front-end matching
		else {
			if (it->getPip2() == t->getPip1()) {
				it->setEligible();
			}
			it->swapPips();
			if (it->getPip2() == t->getPip1()) {
				it->setEligible();
			}
			it->swapPips();
		}
	}
	return;
}

/* *********************************************************************
Function Name: removeIneligibles
Purpose: Remove ineligible tiles from Deck
Parameters:none
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Deck::removeIneligibles() {
	auto it = tileDeck.begin();
	while (it != tileDeck.end()) {
		if (!((*it)->checkEligible())) {
			it = tileDeck.erase(it);
		}
		else {
			//We only increment if tile element is eligible.
			//Otherwise we might go past the range of deque.
			//Especially as elements are deleted.
			++it;
		}
	}
}

/* *********************************************************************
Function Name: checkForDoubles
Purpose: Check if Deck has doubles within it.
Parameters: none
Return Value: true if a double is found, false otherwise.
Algorithm:
Assistance Received: none
********************************************************************* */
bool Deck::checkForDoubles() {
	for (auto it : tileDeck) {
		if (it->checkDouble()) {
			return true;
		}
	}
	return false;
}

/* *********************************************************************
Function Name: findValueOfDeck
Purpose: Find value of all tiles in Deck.
Parameters:none
Return Value: Deck's value in points
Algorithm:
Assistance Received: none
********************************************************************* */
unsigned short Deck::findValueOfDeck() const {
	//Empty decks have no tiles - no value.
	if (tileDeck.empty())return 0;
	unsigned short deckScore = 0;
	for (auto it : tileDeck) {
		deckScore += it->calculateVal();
	}
	return deckScore;
}

/* *********************************************************************
Function Name: setAllEligible
Purpose: Set all tiles to be eligible.
Parameters:none
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Deck::setAllEligible() {
	for (auto it : tileDeck) {
		it->setEligible();
	}
}

/* *********************************************************************
Function Name: resetEligibility
Purpose: Make all tiles ineligible once more.
Parameters:none
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Deck::resetEligibility() {
	for (auto it : tileDeck) {
		it->setIneligible();
	}
}

/* *********************************************************************
Function Name: noEligibleTiles
Purpose: Check if there are any eligible tiles in the deck.
Parameters:none
Return Value: false if any eligible tiles are found, true otherwise.
Algorithm:
Assistance Received: none
********************************************************************* */
bool Deck::noEligibleTiles() {
	for (auto it : tileDeck) {
		if (it->checkEligible()) {
			return false;
		}
	}
	return true;
}

/* *********************************************************************
Function Name: destroyPointers
Purpose: Deallocate Tile pointers in tileDeck
Parameters:
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Deck::destroyPointers() {
	deque<Tile*>::iterator it = tileDeck.begin();
	while (it != tileDeck.end()) {
		delete* it;
		++it;
	}
	tileDeck.clear();
}
/* *********************************************************************
Function Name: destroyPointers2
Purpose: Deallocate all Tile pointers in tileDeck except for first element.
Parameters:none
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Deck::destroyPointers2() {
	deque<Tile*>::iterator it = tileDeck.begin()+1;
	while (it != tileDeck.end()) {
		delete* it;
		++it;
	}
	tileDeck.clear();
}