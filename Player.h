#pragma once
#include "stdafx.h"
#include "Train.h"

class Player {
	public:
		Player() :personalEligible(true), mexEligible(true), opponentEligible(false){}
		inline Train getPersonal() { return personalTrain; }
		inline Train getMexican() { return mexicanTrain; }
		inline Train getOpponent() { return opponentTrain; }
		inline deque<Tile*>getBoneYard() { return boneYard; }
		inline unsigned short getPoints() const { return playerHand.findValueOfDeck(); }
		inline bool emptyHand() { return playerHand.ifEmpty(); }
		void addToDeck(Tile *);
		void removeFromDeck(Tile*);
		inline void showDeck(ostream& os){ playerHand.displayDeck(os); }
		void conditionsSecondDouble(Tile*, Deck&);
		void resetEligibileTrains();
		void destroyHandPointers();
		void getHelp(Deck, unsigned short);
		inline virtual void handleNoEligibles() = 0;
		virtual Deck personalEligibles(Deck) = 0;
		virtual Deck opponentEligibles(Deck) = 0;
		virtual void displayHelp(Tile*, string, string) = 0;
		virtual void checkMexEligible(Deck&) = 0;
		virtual void checkEligibility(Deck&) = 0;
		virtual void playMarker() = 0;
		void loadTrains(Train, Train, Train, deque<Tile*>);
		virtual void playTileOnTrain(string, Tile*) = 0;
		virtual void playTurn(unsigned short) = 0;
		virtual bool determineOrphans(Deck&) = 0;
		virtual unsigned short playerMenu() = 0;
	protected:
		//When playTurn() is called, no tiles have been
		//played yet. So tilesPlayed will be zero.
		unsigned short firstTurn = 0;
		Deck playerHand;
		Train personalTrain, opponentTrain, mexicanTrain;
		deque<Tile*> boneYard;
		//Flags for which trains are eligible for player
		bool personalEligible, opponentEligible, mexEligible;

	private:
};