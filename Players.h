#pragma once
#include <string>
#include "Config.h"

using namespace std;

class Player
{
private:
	char color;
	int piecesCount;
public:
	Player(bool white, int piecesCount);
	int getPieces()const;
	char getColor()const;
	void setPieces(int p);
	void setColor(char c);
};

class Players
{
private:
	Player* current;
	Player* opponent;
public:
	Players(int GW, int GB, char cur);
	~Players();
	void switchPlayers();
	Player*& getCurrent();
	Player*& getOpponent();
};
