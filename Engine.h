#pragma once
#include <string>
#include <iostream>
#include <list>
#include "Config.h"
#include "Solver.h"
#include "Board.h"

using namespace std;

class Engine {
public:
private:
	Solver* solver;
	Board* board;
	string command;
	bool isEngineCommand;
	bool exit;
public:
	Engine() 
	{
		isEngineCommand = true;
		exit = false;
		solver = new Solver(this);
	}
	~Engine() 
	{
		delete solver;
	}
	void inputCommand() 
	{
		cout << ENGINE_PREFIX;
		cin >> command;
	}
	void executeCommand()
	{
		if (command == "")
			return;
		else if (command == EXIT_CMD)
			exit = true;
		else if (command == SWITCH_CMD)
			isEngineCommand = false;
		else if (command == LOAD_GAME_CMD)
			loadGame();
	}
	bool getIsEngineCommand() { return isEngineCommand; }
	void setIsEngineCommand(bool value) {isEngineCommand = value; }
	bool getExit() { return exit;}
	void setExit(bool value) { exit = value; }
	Solver*& getSolver() { return solver; }
private:
	void loadGame()
	{
		delete board;


		board = new Board();
	}
};
