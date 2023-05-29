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
		delete board;
	}
	void inputCommand() 
	{
		if (!(cin >> command)) {
			exit = true;
		}
	}
	void executeCommand()
	{
		if (command == "" || exit)
			return;
		else if (command == EXIT_CMD)
			exit = true;
		else if (command == LOAD_GAME_CMD)
			loadGame();
		else if (command == PRINT_BOARD_CMD)
			printGameBoard();
		else if (command == DO_MOVE_CMD)
			doMove();
		else if (command == GAME_STATE_CMD)
			printGameState();
	}
	bool getExit()const { return exit;}
	void setExit(bool value) { exit = value; }
	
private:
	void loadGame()
	{
		string S, K, GW,GB;
		int s, k, w, b;

		cin >> S >> K >> GW >> GB;
		delete board;

		s = Board::stringToInt(S);
		k = Board::stringToInt(K);
		w = Board::stringToInt(GW);
		b = Board::stringToInt(GB);
		board = new Board(s,k,w,b);
		string boardStatus = board->getBoardStatus();
		if (boardStatus != BOARD_STATUS_OK)
		{
			delete board;
			board = nullptr;
		}
		cout << boardStatus << "\n\n";
	}
	void printGameBoard() const
	{
		if (board == nullptr)
			cout << "Plansza byla niepoprawna lub nie zostala wczytana!!!\n";
		else
			board->print();
	}
	void doMove()
	{
		string coords, start,end;
		string moveStatus = "";
		cin >> coords;
		
		int tmp = coords.find('-');
		start = coords.substr(0, tmp);
		end = coords.substr(tmp+1, coords.size());

		moveStatus = board->checkMove(start, end);
		cout << moveStatus << endl << endl;
	}
	void printGameState() const
	{
		cout << board->getGameState() << endl << endl;
	}
};
