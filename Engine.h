#pragma once
#include <string>
#include <iostream>
#include "Config.h"
#include "Board.h"

using namespace std;

class Engine {
private:
	Board* board;
	string command;
	string gameState;
	bool exit;
public:
	Engine();
	~Engine();

	void inputCommand();
	void executeCommand();

	bool getExit() const;
	void setExit(bool value);	
private:
	void loadGame();
	void printGameBoard() const;
	void printGameState() const;
	void doMove();
};
