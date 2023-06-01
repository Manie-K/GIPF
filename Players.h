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
	Player(const Player& other);
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
	Players(const Players& other);
	Players(const Player& cur, const Player& opp);
	~Players();
	void switchPlayers();
	Player*& getCurrent();
	Player*& getOpponent();
};
