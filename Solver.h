#pragma once
#include <iostream>
#include <string>
#include"Config.h"

using namespace std;

class Solver
{
public:
private:
	Engine* engine;
	string command;
public:
	Solver(Engine* e):engine(e) {}
	~Solver(){}
	void inputCommand()
	{
		cout << SOLVER_PREFIX;
		cin >> command;
	}
	void executeCommand()
	{
		if (command == "")
			return;
		else if (command == EXIT_CMD)
			engine->setExit(true);
		else if (command == SWITCH_CMD)
			engine->setIsEngineCommand(false);
	}
private:

};

