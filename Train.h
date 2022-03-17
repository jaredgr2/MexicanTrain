#pragma once
#include "stdafx.h"
#include "Deck.h"

class Train : public Deck {
	public:
		Train() { markedTrain = false; containsOrphan = false; }
		Train(const Train& t);
		~Train();
		bool ifMarked()const { return markedTrain; }
		bool ifOrphan() const { return containsOrphan; }
		Tile* getTileAtFront() const { return tileDeck.front(); }
		Tile* getTileAtBack() const { return tileDeck.back(); }
		void hasOrphan() { containsOrphan = true; }
		void notOrphan() { containsOrphan = false; }
		void addToBack(Tile*);
		void addToFront(Tile*);
		bool checkTileMatchFront(Tile *);
		bool checkTileMatchBack(Tile *);
		void checkMarkedTrain();
		void unmarkTrain();
		void compareToPrevious(Tile*);
		void compareToFront(Tile*);
	protected:

	private:
		//Marked train flag and orphan double flag.
		bool markedTrain, containsOrphan;
};