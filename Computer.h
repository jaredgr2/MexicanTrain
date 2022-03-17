#pragma once
#include "stdafx.h"
#include "Player.h"

class Computer : public Player {
	public:
		Computer() {}
		Computer(Deck);
		void checkEligibility(Deck &);
		bool determineOrphans(Deck &);
		void displayHelp(Tile*, string, string);
		void handleNoEligibles();
		Deck personalEligibles(Deck);
		Deck opponentEligibles(Deck);
		void checkMexEligible(Deck &);
		void playMarker();
		void playTurn(unsigned short);
		void playTileOnTrain(string, Tile*);
		unsigned short playerMenu();
	protected:

	private:
		//Player choices - save, make a move, or quit
		unsigned short SAVE = 1;
		unsigned short CONTINUEGAME = 2;
		unsigned short QUIT = 3;

};