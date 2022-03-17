#pragma once

class Tile {
	public:
		Tile();
		Tile(unsigned short, unsigned short);
		Tile(char);
		Tile(string);
		unsigned short getPip1()const { return pip1; }
		unsigned short getPip2()const { return pip2; }
		bool checkEligible()const { return isEligible; }
		bool checkMarker()const { return markerTile; }
		bool checkDouble()const { return isDouble; }
		void setPip1(unsigned short p) { pip1 = p; }
		void setPip2(unsigned short p) { pip2 = p; }
		void setEligible() { isEligible = true; }
		void setIneligible() { isEligible = false; }
		void print(ostream&);
		void swapPips();
		unsigned short calculateVal();
	protected:
	private:
		//Left and right hand pip, respectively.
		unsigned short pip1, pip2;
		//Eligible tile flag
		bool isEligible;
		//Marker flag
		bool markerTile;
		//Double flag
		bool isDouble;
};
