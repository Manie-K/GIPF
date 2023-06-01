#include "Players.h"

Player::Player(bool white, int piecesCount)
{
	this->piecesCount = piecesCount;
	if (white)
		color = WHITE_PIECE;
	else
		color = BLACK_PIECE;
}
Player::Player(const Player& other)
{
	this->color = other.color;
	this->piecesCount = other.piecesCount;
}
int Player::getPieces()const { return piecesCount; }
char Player::getColor()const { return color; }
void Player::setPieces(int p) { piecesCount = p; }
void Player::setColor(char c) { color = c; }

Players::Players(int GW, int GB, char cur)
{
	current = new Player(true, GW);
	opponent = new Player(false, GB);
	if (cur == BLACK_PIECE)
		switchPlayers();
}
Players::Players(const Player& cur, const Player& opp)
{
	current = new Player(cur);
	opponent = new Player(opp);
}
Players::~Players()
{
	delete current;
	current = nullptr;
	delete opponent;
	opponent = nullptr;
}
void Players::switchPlayers()
{
	char tempColor = current->getColor();
	int tempPieces = current->getPieces();
	current->setColor(opponent->getColor());
	current->setPieces(opponent->getPieces());
	opponent->setColor(tempColor);
	opponent->setPieces(tempPieces);
}
Player*& Players::getCurrent() { return current; }
Player*& Players::getOpponent() { return opponent; }