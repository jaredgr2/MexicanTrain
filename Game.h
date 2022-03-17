#pragma once
#include "stdafx.h"
#include "Round.h"

class Game {
	public:
		Game():roundNumber(1),humanScore(0),compScore(0),humanPoints(0),compPoints(0), gameTask(0){}
		void gameHandler();
		void runGame();
		void displayRoundEnd();
		void displayGameEnd();
		char checkContinueGame();
		bool askLoadGame();
		bool loadGame(string);
	protected:
	private:
		unsigned short roundNumber, humanScore, compScore, humanPoints, compPoints, gameTask;
		//Player choices, make a move/continue playing, get help, or quit
		unsigned short CONTINUEGAME = 2;
		unsigned short HELP = 3;
		unsigned short QUIT = 4;
		//Objects to have data loaded into them by loadGame() - hence the l_ 
		Deck l_compDec, l_humanDec;
		Train l_compT, l_humanT, l_mexT;
		deque<Tile*> l_boneY;
		string l_nextPlayer;
};