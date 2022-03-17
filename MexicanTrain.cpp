/************************************************************
* Name:  Jared Rodriguez
* Project : Mexican Train (C++)
* Class : CMPS 366 01
* Date : 9/29/21
************************************************************/
#include "stdafx.h"
#include "Game.h"

int main()
{
	//For random generation
	srand(time(NULL));
	//Game object
	Game g;
	//Run the game
	g.gameHandler();
}
