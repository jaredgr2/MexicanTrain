#pragma once
#include "stdafx.h"
#include "Round.h"

/* *********************************************************************
Function Name: Round
Purpose: Overriden Round constructor that sets round number, playerScore,
and compScore using passed parameters.
Parameters:
	r, unsigned short copied into roundNum
	p1, unsigned short copied into playerScore
	p2, unsigned short copied into compScore
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
Round::Round(unsigned short r, unsigned short p1, unsigned short p2) {
	roundNum = r;
	playerScore = p1;
	compScore = p2;
	playerChoice = 0;
	nextPlayer = "";
	//Game is not a resumed one, so false.
	resumedGame = false;
	unsigned short roundModulo = roundNum%10;
	//On every tenth round, engine is 0-0
	if (roundModulo == 0) {
		engineDisplay = "0-0";
	}
	//On other rounds, engine is determined by ten minus round number
	//modulo ten.
	else {
		unsigned short pipNum = 10 - (roundModulo);
		engineDisplay = to_string(pipNum) + "-" + to_string(pipNum);
	}
	loadTiles();
	determineStartingPlayer();
}

/* *********************************************************************
Function Name: Round
Purpose: Overriden Round constructor that sets Round members to passed
parameters.
Parameters:
	r, unsigned short copied into roundNum
	p1, unsigned short copied into playerScore
	p2, unsigned short copied into compScore
	np, string copied into nextPlayer
	ht, Train copied into playerTrain
	ct, Train copied into computerTrain
	mt, Train copied into mexicanTrain
	by, deque<Tile*> copied into boneYard
	h, Deck used to construct Human player
	c, Deck used to construct Computer player
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
Round::Round(unsigned short r, unsigned short p1, unsigned short p2, string np, Train ht, Train ct, Train mt, deque<Tile*> by, Deck h, Deck c) {
	roundNum = r;
	playerScore = p1;
	compScore = p2;
	//Initialize player choice with zero, an impossible choice in menu
	playerChoice = 0;
	nextPlayer = np;
	playerTrain = ht;
	computerTrain = ct;
	mexicanTrain = mt;
	boneYard = by;
	//Set resumedGame flag 
	resumedGame = true;
	//Construct players with passed hands
	humanPlayer = Human(h);
	compPlayer = Computer(c);
	//Engine will always be the same as front of human player's train
	engine = ht.getTileAtFront();
}

/* *********************************************************************
Function Name:~Round
Purpose: Round destructor - frees up memory 
Parameters: none
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
Round::~Round() {
	mexicanTrain.destroyPointers();
	computerTrain.destroyPointers();
	//If a game is resumed, player engine tile points to a different
	//block of memory than computer engine tile.
	if (resumedGame) playerTrain.destroyPointers();
	//Otherwise, we can't destroy the pointer that has already been 
	//deleted, so we destroy all other pointers in playerTrain.
	else playerTrain.destroyPointers2();
	//Delete the boneyard pointers
	deque <Tile*> ::iterator it = boneYard.begin();
	while (it != boneYard.end()) {
		delete* it;
		++it;
	}
	humanPlayer.destroyHandPointers();
	compPlayer.destroyHandPointers();
}

/* *********************************************************************
Function Name: loadTiles
Purpose: Generate round's tiles and pass them to various containers.
Parameters: none
Return Value: none
Algorithm:
	1. Create local Deck object.
	2. Generate new Tiles - all tiles in a Mexican Train game.
	3. Get engine - tile that matches engineDisplay - and push to
	player and computer trains.
	4. Remove engine from local Deck, then shuffle deck.
	5. Push 16 tiles to human deck, 16 to computer deck,
	and remainder to boneyard. Return.
Assistance Received: none
********************************************************************* */
void Round::loadTiles() {
	Deck tiles;
	for (int i = 0; i < 10; i++) {
		for (int j = i; j < 10; j++) {
			tiles.addToBack(new Tile(i, j));
		}
	}
	engine = tiles.getTile(tiles.findTile(engineDisplay));
	playerTrain.addToFront(engine);
	computerTrain.addToBack(engine);
	tiles.removeFromDeck(engine);
	tiles.shuffleDeck();
	deque<Tile*>::const_iterator it = tiles.getBegin();
	for (int i = 0; i < 16; i++ ) {
		humanPlayer.addToDeck(tiles.getTile(it));
		it++;
	}
	for (int i = 16; i < 32; i++) {
		compPlayer.addToDeck(tiles.getTile(it));
		it++;
	}
	for (int i = 32; i < 54; i++) {
		boneYard.push_back(tiles.getTile(it));
		it++;
	}
}

/* *********************************************************************
Function Name: displayRoundInfo
Purpose: Prints round information to ostream.
Parameters:
	ostream os, which is passed by reference and is changed by
	the characters being added to the output stream.
	inGame, boolean flag that determines what will be printed.
Return Value: none
Algorithm:
	1.If inGame is true, display the table.
	2. Display round num, Computer info, Human info, and Mexican train.
	3. If inGame is true, print only first tile of boneyard. Otherwise,
	print all the tiles in boneyard.
	4. Print next player, and return.
Assistance Received: none
********************************************************************* */
void Round::displayRoundInfo(ostream &os, bool inGame) {

	if (inGame) {
		os << "===========Table===========" << endl;
		computerTrain.displayDeck(os);
		playerTrain.displayDeck2(os);
		os << endl << endl;
		mexicanTrain.displayDeck(os);
		os << endl << endl;
	}
	os << "Round: " << roundNum << endl << endl;
	os << "Computer:" << endl;
	os << "	Score: " << compScore << endl;
	os << "	Hand: ";
	compPlayer.showDeck(os);
	os << endl;
	os << "	Train: ";
	computerTrain.displayDeck(os);
	os << endl << endl;
	os << "Human:" << endl;
	os << "	Score: " << playerScore << endl;
	os << "	Hand: ";
	humanPlayer.showDeck(os);
	os << endl;
	os << "	Train: ";
	playerTrain.displayDeck(os);
	os << endl << endl;
	os << "Mexican Train:";
	mexicanTrain.displayDeck(os);
	os << endl << endl;
	os << "Boneyard: ";
	if (inGame) {
		if(boneYard.size()>1) boneYard.front()->print(os);
	}
	else {
		for (auto it : boneYard) {
			it->print(os);
		}
	}
	os << endl << endl;
	os << "Next Player: " << nextPlayer << endl;
	return;
}

/* *********************************************************************
Function Name: determineStartingPlayer
Purpose: Determines which player starts the round.
Parameters: none
Return Value: none
Algorithm:
	1. If no next player is set, check players scores
	2. If compScore is less than playerScore, computer goes first,
	if greater than playerScore, player goes first. Return on either.
	3. If neither, have player call a coin flip.
	4. If they win the coin flip, they go first. Otherwise, computer
	goes first.
	5. Return.
Assistance Received: none
********************************************************************* */
void Round::determineStartingPlayer() {
	if (nextPlayer == "") {
		if (compScore < playerScore) {
			nextPlayer = "Computer";
			return;
		}
		else if (compScore > playerScore) {
			nextPlayer = "Human";
			return;
		}
		else {
			char coinDecision;
			do {
				cout << "A coin will be flipped to decide who goes first.Heads or tails? (h/t)";
				cin >> coinDecision;
				coinDecision = tolower(coinDecision);
			} while (coinDecision != 'h' && coinDecision != 't');
			char simulateCoin = coinToss();
			if (coinDecision == simulateCoin) {
				nextPlayer = "Human";
				cout << "You won the coin toss with " << coinDecision << endl;
				return;
			}
			else {
				nextPlayer = "Computer";
				cout << "You lost the coin toss with " << coinDecision << endl;
				return;
			}
		}
	}
}

/* *********************************************************************
Function Name: coinToss
Purpose: Simulates coin toss
Parameters: none
Return Value: a char which is 'h' if heads wins, 't' if tails wins.
Algorithm:
Assistance Received: none
********************************************************************* */
char Round::coinToss() {
	int randNum = 1 + rand() % 2;
	if (randNum == 1)return 'h';
	else return 't';
}

/* *********************************************************************
Function Name: updateBoard
Purpose: Updates the game board.
Parameters: none
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Round::updateBoard() {
	if (nextPlayer == "Human") {
		playerTrain = humanPlayer.getPersonal();
		computerTrain = humanPlayer.getOpponent();
		mexicanTrain = humanPlayer.getMexican();
		boneYard = humanPlayer.getBoneYard();
	}
	if (nextPlayer == "Computer") {
		computerTrain = compPlayer.getPersonal();
		playerTrain = compPlayer.getOpponent();
		mexicanTrain = compPlayer.getMexican();
		boneYard = compPlayer.getBoneYard();
	}
	setOrphanTrains();
	return;
}

/* *********************************************************************
Function Name: setOrphanTrains
Purpose: Set orphan flags on trains which have orphan doubles.
Parameters: none
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Round::setOrphanTrains() {
	//We don't count the engine as an orphan double.
	if (computerTrain.getTileAtFront()->checkDouble() && computerTrain.getTileAtFront()->getPip1() != engine->getPip1()) {
		computerTrain.hasOrphan();
	}
	if (playerTrain.getTileAtBack()->checkDouble() && playerTrain.getTileAtBack()->getPip1() != engine->getPip1()) {
		playerTrain.hasOrphan();
	}
	if (mexicanTrain.ifEmpty()) {
		return;
	}
	else if (mexicanTrain.getTileAtBack()->checkDouble() && mexicanTrain.getTileAtBack()->getPip1() != engine->getPip1()) {
		mexicanTrain.hasOrphan();
	}
	return;
}

/* *********************************************************************
Function Name: runRound
Purpose: Runs the game loop for round.
Parameters: none
Return Value: none
Algorithm:
	1. Display round info.
	2. Set any orphan trains.
	3. Run turn depending on which player is nextPlayer at the time.
	4. Load trains and boneyard into current player.
	5. Display player menu and read in choice.
	6. If player chooses to quit, return QUIT, if player chooses to save,
	call saveGame() and return QUIT. If human player chooses help, call 
	callHelp().
	7. Run player's turn and update board, then display round info. Set
	next player to the other player.
	8. Continue having each player take their turn while end conditions
	haven't been met or player hasn't chosen to quit.
	9.If end condition has been met, return CONTINUEGAME.
Assistance Received: none
********************************************************************* */
unsigned short Round::runRound() {
	displayRoundInfo(cout,true);
	setOrphanTrains();
	do {
		if (nextPlayer == "Human") {
			humanPlayer.loadTrains(playerTrain, computerTrain, mexicanTrain, boneYard);
			playerChoice = humanPlayer.playerMenu();
			if (playerChoice == QUIT)return QUIT;
			if (playerChoice == SAVE) {
				saveGame();
				return QUIT;
			}
			if (playerChoice == HELP)humanPlayer.callHelp();
			humanPlayer.playTurn(firstTurn);
			updateBoard();
			displayRoundInfo(cout,true);
			nextPlayer = "Computer";
		}
		if (nextPlayer == "Computer") {
			compPlayer.loadTrains(computerTrain, playerTrain, mexicanTrain, boneYard);
			playerChoice = compPlayer.playerMenu();
			if (playerChoice == QUIT)return QUIT;
			if (playerChoice == SAVE) {
				saveGame();
				return QUIT;
			}
			compPlayer.playTurn(firstTurn);
			updateBoard();
			displayRoundInfo(cout, true);
			nextPlayer = "Human";
		}
	} while (!(endConditionsMet()));
	return CONTINUEGAME;
}

/* *********************************************************************
Function Name: saveGame
Purpose: Saves game state into text file using displayRoundInfo and an 
ostream.
Parameters: none
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Round::saveGame() {
	string fileName;
	//Prompt user for filename
	cout << "What name would you like to save the game's file under?" << endl;
	cin >> fileName;
	//Create output stream and open it under user's chosen name with .txt extension (text file)
	ofstream newFile;
	newFile.open(fileName + ".txt");
	//Call displayRoundInfo() and pass output stream to it, which prints round info to text file, then close filestream.
	displayRoundInfo(newFile,false);
	newFile.close();
	return;
}

/* *********************************************************************
Function Name: endConditionsMet
Purpose: Checks and returns if end conditions for round have been met.
Parameters: none
Return Value: bool value, true if end conditions met, false otherwise.
Algorithm:
Assistance Received: none
********************************************************************* */
bool Round::endConditionsMet() {
	if (humanPlayer.emptyHand() || compPlayer.emptyHand()) return true;
	if (playerTrain.ifMarked() && computerTrain.ifMarked() && boneYard.empty()) return true;
	return false;
}