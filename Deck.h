#pragma once
#include "stdafx.h"
#include "Tile.h"

class Deck {
	public:
		Deck() {};
		Deck(const Deck& d);
		~Deck();
		inline deque<Tile*>::const_iterator getBegin()const { return tileDeck.begin(); }
		inline deque<Tile*>::const_iterator getMarkedBegin()const { return tileDeck.begin()+1; }
		inline deque<Tile*>::const_iterator getEnd()const{ return tileDeck.end(); }
		inline deque<Tile*>::const_iterator getMarkedEnd()const { return tileDeck.end()-2; }
		inline bool ifEmpty() const { return tileDeck.empty(); }
		deque<Tile*>::const_iterator findTile(string);
		Tile* getTile(deque<Tile*>::const_iterator) const;
		Tile* findLargestTile() const;
		Tile* findLargestDouble() const;
		unsigned short findValueOfDeck() const;
		bool noEligibleTiles();
		virtual void addToBack(Tile*);
		void displayDeck(ostream&);
		void displayDeck2(ostream&);
		void shuffleDeck();
		void emptyDeck();
		void removeFromDeck(Tile *);
		void checkTilesEligible(Tile*, string);
		void removeIneligibles();
		bool checkForDoubles();
		void setAllEligible();
		void resetEligibility();
		void destroyPointers();
		void destroyPointers2();
	protected:
		//Deck of tiles to be stored
		deque<Tile*>tileDeck;
	private:

};