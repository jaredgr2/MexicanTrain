#pragma once
#include "stdafx.h"
#include "Computer.h"
#include "Human.h"

class Round {
	public:
		Round() :roundNum(0), compScore(0), playerScore(0), playerChoice(0), nextPlayer(""), 
			engineDisplay(""),resumedGame(false) {}
		Round(unsigned short, unsigned short, unsigned short);
		Round(unsigned short, unsigned short, unsigned short, string, Train, Train, Train, deque<Tile*>, Deck, Deck);
		~Round();
		inline unsigned short getCompPoints() const  { return compPlayer.getPoints(); }
		inline unsigned short getHumanPoints() const { return humanPlayer.getPoints(); }
		void loadTiles();
		void displayRoundInfo(ostream &, bool);
		void determineStartingPlayer();
		char coinToss();
		void updateBoard();
		void setOrphanTrains();
		unsigned short runRound();
		void saveGame();
		bool endConditionsMet();
	protected:

	private:
		//When playTurn() is called, no tiles have been
		//played yet. So tilesPlayed will be zero.
		unsigned short firstTurn = 0;
		unsigned short roundNum, compScore, playerScore, playerChoice;
		//Player choices - save, make a move, get help, or quit
		unsigned short SAVE = 1;
		unsigned short CONTINUEGAME = 2;
		unsigned short HELP = 3;
		unsigned short QUIT = 4;
		//Flag to check if game is created from text file or is fresh game
		bool resumedGame;
		//String to help find engine, and next player for turn
		string engineDisplay, nextPlayer;
		Computer compPlayer;
		Human humanPlayer;
		Train playerTrain,computerTrain,mexicanTrain;
		Tile* engine = new Tile;
		deque<Tile*> boneYard;
};