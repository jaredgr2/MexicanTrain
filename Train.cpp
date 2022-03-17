#pragma once
#include "stdafx.h"
#include "Train.h"

/* *********************************************************************
Function Name: ~Train
Purpose: Train destructor
Parameters:none
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
Train::~Train() {
	tileDeck.clear();
}

/* *********************************************************************
Function Name: Train
Purpose: Train copy constructor
Parameters:
	Train t, a Train object passed by const reference. It is not changed.
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
Train::Train(const Train& t) {
	tileDeck = t.tileDeck;
	containsOrphan = t.containsOrphan;
	markedTrain = t.markedTrain;
}
/* *********************************************************************
Function Name: addToBack
Purpose: Add given tile to back of Train.
Parameters:
	t, a Tile* which will be pushed to back of tileDeck.
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Train::addToBack(Tile* t) {
	//We do not compare to previous if the tileDeck is empty
	//since there is no previous to compare to.
	if (tileDeck.empty()) {
		tileDeck.push_back(t);
		return;
	}
	compareToPrevious(t);
	tileDeck.push_back(t);
	//If the train has an orphan double and it gets played on at this time,
	//we set the orphan flag to false. It is no longer orphan double.
	if (containsOrphan) notOrphan();
	return;
}

/* *********************************************************************
Function Name: addToFront
Purpose: Add given tile to front of Train.
Parameters:
	t, a Tile* which will be pushed to front of tileDeck.
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Train::addToFront(Tile * t) {
	//We do not compare to the front if the tileDeck is empty
	//since there is no front to compare to.
	if (tileDeck.empty()) {
		tileDeck.push_front(t);
		return;
	}
	compareToFront(t);
	tileDeck.push_front(t);
	//If the train has an orphan double and it gets played on at this time,
	//we set the orphan flag to false. It is no longer orphan double.
	if (containsOrphan) notOrphan();
	return;
}

/* *********************************************************************
Function Name: checkTileMatchFront
Purpose: Check if given tile matches the front of a Train.
Parameters:
	t, a Tile* which will be compared to front of a Train.
Return Value: true if t matches front, false otherwise.
Algorithm:
Assistance Received: none
********************************************************************* */
bool Train::checkTileMatchFront(Tile* t) {
	//If marker is at front of Train, compare tile to 
	//the tile to the next of the marker.
	if (tileDeck.front()->checkMarker()) {
		if (t->getPip1() == tileDeck[1]->getPip1() || t->getPip2() == tileDeck[1]->getPip1()) {
			return true;
		}
		return false;
	}
	else {
		if (t->getPip1() == (tileDeck.front())->getPip1() || t->getPip2() == (tileDeck.front())->getPip1()) {
			return true;
		}
	}
	return false;
}

/* *********************************************************************
Function Name: checkTileMatchBack
Purpose: Check if given tile matches the back of a Train.
Parameters:
	t, a Tile* which will be compared to back of a Train.
Return Value: true if t matches back, false otherwise.
Algorithm:
Assistance Received: none
********************************************************************* */
bool Train::checkTileMatchBack(Tile* t) {
	//If tileDeck is empty, check against front - applicable to 
	//Mexican train only.
	if (tileDeck.size() < 1) {
		return checkTileMatchFront(t);
	}
	deque<Tile*>::iterator it = tileDeck.end() - 2;
	//If marker is at back of Train, compare tile to
	//the tile behind the marker.
	if (tileDeck.back()->checkMarker()) {
		if (t->getPip1() == (*it)->getPip2() || t->getPip2() == (*it)->getPip2()) {
			return true;
		}
		return false;
	}
	else {
		if (t->getPip1() == (tileDeck.back())->getPip2() || t->getPip2() == (tileDeck.back())->getPip2()) {
			return true;
		}
	}
	return false;
}

/* *********************************************************************
Function Name: checkMarkedTrain
Purpose: Check if train has marker and if so sets boolean flag.
Parameters: none
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Train::checkMarkedTrain() {
	for (auto it : tileDeck) {
		if (it->checkMarker()) {
			markedTrain = true;
		}
	}
	return;
}

/* *********************************************************************
Function Name: unmarkTrain
Purpose: Removes marker tile from train and sets boolean flag to false.
Parameters: none
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Train::unmarkTrain() {
	markedTrain = false;
	for (auto it : tileDeck) {
		if (it->checkMarker()) {
			removeFromDeck(it);
		}
	}

}

/* *********************************************************************
Function Name: compareToPrevious
Purpose: Swaps pips if given tile's first pip isn't equal to second pip of
previous Tile on Train but its second pip is equal.
Parameters:
	t, a Tile* which will be compared to previous Tile* on Train.
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Train::compareToPrevious(Tile* t) {
	if (tileDeck.size()>4) {
		deque <Tile*> ::iterator it = tileDeck.end() - 2;
		if (tileDeck.back()->checkMarker()) {
			if (t->getPip1() != (*it)->getPip2() && t->getPip2() == (*it)->getPip2()) {
				t->swapPips();
			}
		return;
		}
	}
	if (t->getPip1() != tileDeck.back()->getPip2() && t->getPip2() == tileDeck.back()->getPip2()) {
		t->swapPips();
	}
	return;
}

/* *********************************************************************
Function Name: compareToFront
Purpose: Swaps pips if given tile's second pip isn't equal to first pip of
front Tile on Train but its first pip is equal.
Parameters:
	t, a Tile* which will be compared to front Tile* on Train.
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Train::compareToFront(Tile* t) {
	if (tileDeck.front()->checkMarker()) {
		if (t->getPip2() != tileDeck[1]->getPip1() && t->getPip1() == tileDeck[1]->getPip1()) {
			t->swapPips();
		}
		return;
	}
	if (t->getPip2() != tileDeck.front()->getPip1() && t->getPip1() == tileDeck.front()->getPip1()) {
		t->swapPips();
	}
	return;
}
