#pragma once
#include <string>
#include <iostream>
#include <list>
#include "Config.h"
#include "Board.h"

using namespace std;

class Engine {
public:
private:
	Board* board;
	string command;
	string gameState;
	bool exit;
public:
	Engine() 
	{
		board = nullptr;
		command = "";
		exit = false;
		gameState = GAME_STATE_PROGRESS;
	}
	~Engine() 
	{
	}
	void inputCommand() 
	{
		cin >> command;
	}
	void executeCommand()
	{
		if (command == "")
			return;
		else if (command == EXIT_CMD)
			exit = true;
		else if (command == LOAD_GAME_CMD)
			loadGame();
		else if (command == PRINT_BOARD_CMD)
			printGameBoard();
	}
	bool getExit() { return exit;}
	void setExit(bool value) { exit = value; }
private:
	void loadGame()
	{
		string S, K, GW,GB;
		cin >> S >> K >> GW >> GB;
		delete board;
		board = new Board(stoi(S), stoi(K), stoi(GW), stoi(GB));
		string boardStatus = board->getBoardStatus();
		if (boardStatus != BOARD_STATUS_OK)
		{
			delete board;
			board = nullptr;
		}
		cout << boardStatus << "\n";
	}
	void printGameBoard()
	{
		if (board == nullptr)
			cout << "Plansza byla niepoprawna lub nie zostala wczytana!!!\n";
		else
			board->print();
	}
};
