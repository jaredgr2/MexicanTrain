#pragma once
#include "stdafx.h"
#include "Tile.h"

/* *********************************************************************
Function Name: Tile
Purpose: Default Tile constructor
Parameters: none
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
Tile::Tile() {
	pip1 = 10;
	pip2 = 11;
	isEligible = false;
	markerTile = false;
	isDouble = false;
}

/* *********************************************************************
Function Name: Tile
Purpose: Tile constructor for non-marker tiles
Parameters: 
	unsigned short first - is set to the first pip of Tile and
	unsigned short second - set to the second pip of Tile
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
Tile::Tile(unsigned short first, unsigned short second) {
	//All tiles are ineligible at the beginning
	isEligible = false;
	markerTile = false;
	pip1 = first;
	pip2 = second;
	//If double, set boolean flag
	if (pip1 == pip2) {
		isDouble = true;
	}
	else {
		isDouble = false;
	}
}

/* *********************************************************************
Function Name: Tile
Purpose: Tile constructor for marker tile exclusively
Parameters: 
	char marker, which just indicates that this constructor is being called if it
	is equal to 'm' or 'M'
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
Tile::Tile(char marker) {
	//Make char lowercase if not already
	marker = tolower(marker);
	//Validate char passed
	if (marker != 'm') {
		cerr << "Improper value for marker" << endl;
	}
	//Pips must be set
	pip1 = 10;
	pip2 = 11;
	isEligible = false;
	isDouble = false;
	markerTile = true;
}

/* *********************************************************************
Function Name: Tile
Purpose: Tile constructor for serialization
Parameters: 
	string s, which is read and creates either a marker (if equal to "M")
	or nonmarker valid tile using substrings to load pips.
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
Tile::Tile(string s) {
	isEligible = false;
	if (s == "M") {
		pip1 = 10;
		pip2 = 11;
		isDouble = false;
		markerTile = true;
	}
	else {
		string pip1str = s.substr(0, 1);
		string pip2str = s.substr(2);
		pip1 = (unsigned short)stoul(pip1str, nullptr, 10);
		pip2 = (unsigned short)stoul(pip2str, nullptr, 10);
		markerTile = false;
		if (pip1 == pip2) {
			isDouble = true;
		}
		else isDouble = false;
	}
}

/* *********************************************************************
Function Name: print
Purpose: Prints tile to ostream
Parameters: 
	ostream os, which is passed by reference and is changed by
	the characters being added to the output stream.
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Tile::print(ostream& os) {
	if (markerTile) {
		os << "M ";
	}
	else {
		os << pip1 << "-" << pip2 << " ";
	}
	return;
}

/* *********************************************************************
Function Name: swapPips
Purpose: Swaps pip values of pip1 and pip2
Parameters: none
Return Value: none
Algorithm:
Assistance Received: none
********************************************************************* */
void Tile::swapPips() {
	//Store pip1 value in temp var then swap vals
	unsigned short temp;
	temp = pip1;
	pip1 = pip2;
	pip2 = temp;
}

/* *********************************************************************
Function Name: calculateVal
Purpose: Calculates value of tile
Parameters: none
Return Value: sum of pips of tile, or zero if tile is marker.
Algorithm:
Assistance Received: none
********************************************************************* */
unsigned short Tile::calculateVal() {
	//Marker tiles have no value even though they have pips
	if (markerTile) {
		return 0;
	}
	else {
		return pip1 + pip2;
	}
}