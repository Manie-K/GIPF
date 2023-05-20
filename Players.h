#pragma once
#include <string>
#include "Config.h"

using namespace std;

class Player
{
public:
private:
	string color;
	int piecesCount;
public:
	Player(bool white, int piecesCount)
	{
		this->piecesCount = piecesCount;
		if (white)
			color = WHITE_PIECE;
		else
			color = BLACK_PIECE;
	}
	~Player(){}
	int getPieces()const { return piecesCount; }
	string getColor()const { return color; }
	void setPieces(int p) { piecesCount = p; }
	void setColor(string c) { color = c; }
private:

};

class Players
{
public:
private:
	Player* current;
	Player* opponent;
public:
	Players(int GW, int GB, char cur)
	{
		current = new Player(true, GW);
		opponent = new Player(false, GB);
		if (cur == BLACK_PIECE)
			switchPlayers();
	}
	~Players()
	{
		delete current;
		current = nullptr;
		delete opponent;
		opponent = nullptr;
	}
	void switchPlayers()
	{
		string tempColor = current->getColor();
		int tempPieces = current->getPieces();
		current->setColor(opponent->getColor());
		current->setPieces(opponent->getPieces());
		opponent->setColor(tempColor);
		opponent->setPieces(tempPieces);
	}
private:
};
