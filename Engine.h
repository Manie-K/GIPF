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
		cin >> command;
		if (cin.eof())
			exit = true;
	}
	void executeCommand()
	{
		if (exit || command == "")
			return;
		else if (command == EXIT_CMD)
			exit = true;
		else if (command == LOAD_GAME_CMD)
			loadGame();
		else if (command == PRINT_BOARD_CMD)
			printGameBoard();
		else if (command == DO_MOVE_CMD)
			doMove();
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

		s = stringToInt(S);
		k = stringToInt(K);
		w = stringToInt(GW);
		b = stringToInt(GB);
		board = new Board(s,k,w,b);
		string boardStatus = board->getBoardStatus();
		if (boardStatus != BOARD_STATUS_OK)
		{
			delete board;
			board = nullptr;
		}
		cout << boardStatus << "\n";
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
		end = coords.substr(tmp, coords.size());

		moveStatus = board->checkMove(start, end);
		if (moveStatus == MOVE_STATUS_OK)
			board->move(start, end);
		cout << moveStatus << '\n';
	}

	static int stringToInt(const string& str)
	{
		int ret = 0;
		int sign = 1;
		int i = 0;

		if (str[0] == '-') {
			sign = -1;
			i++;
		}
		else if (str[0] == '+') {
			i++;
		}

		while (i < str.length()) {
			if (str[i] >= '0' && str[i] <= '9') {
				int digit = str[i] - '0';
				ret = ret * 10 + digit;
			}
			else {
				break;
			}
			i++;
		}

		return ret * sign;
	}
};
