#pragma once
#include "stdafx.h"
#include "Player.h"

class Human : public Player {
	public:
		Human() {}
		Human(Deck);
		Tile* playTile();
		string verifyTrain(Tile*);
		string selectEligibleTrain();
		string selectTrain();
		Deck personalEligibles(Deck);
		Deck opponentEligibles(Deck);
		inline void callHelp() { getHelp(playerHand, firstTurn); }
		inline void handleNoEligibles() { cout << "No eligible tiles found! You will have to draw from boneyard." << endl; }
		void displayHelp(Tile*, string, string);
		void checkMexEligible(Deck&);
		void checkEligibility(Deck&);
		bool boneYardDraw();
		void playMarker();
		void playTileOnTrain(string, Tile*);
		void playTurn(unsigned short);
		bool determineOrphans(Deck&);
		unsigned short playerMenu();
	protected:

	private:
		//Player choices - save, make a move, get help, or quit
		unsigned short SAVE = 1;
		unsigned short CONTINUEGAME = 2;
		unsigned short HELP = 3;
		unsigned short QUIT = 4;
};