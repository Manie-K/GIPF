#include "Engine.h"

Engine::Engine() :board(nullptr), command(""), exit(false) {}
Engine::~Engine() { delete board; }

void Engine::inputCommand()
{
	if (!(cin >> command)) {
		exit = true;
	}
}
void Engine::executeCommand()
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
	else if (command == GAME_STATE_CMD || command == GAME_STATE_STOS_CMD) {
		allMoves(false, false, true);
		printGameState();
	}
	else if (command == ALL_MOVES_CMD)
		allMoves(false, false);
	else if (command == ALL_MOVES_NUMBER_CMD)
		allMoves(true, false);
	else if (command == ALL_MOVES_WIN_CMD)
		allMoves(false, true);
	else if (command == ALL_MOVES_WIN_NUMBER_CMD)
		allMoves(true, true);
}

bool Engine::getExit()const { return exit; }
void Engine::setExit(bool value) { exit = value; }

void Engine::loadGame()
{
	string S, K, GW, GB;
	int s, k, w, b;

	cin >> S >> K >> GW >> GB;
	delete board;

	s = Board::stringToInt(S);
	k = Board::stringToInt(K);
	w = Board::stringToInt(GW);
	b = Board::stringToInt(GB);
	board = new Board(s, k, w, b);

	string boardStatus = board->getBoardStatus();
	if (boardStatus != BOARD_STATUS_OK)
	{
		delete board;
		board = nullptr;
	}
	cout << boardStatus << endl;
}
void Engine::printGameBoard() const
{
	if (board == nullptr)
		cout << BOARD_STATUS_EMPTY << endl;
	else
		board->print();
}
void Engine::printGameState() const
{
	cout << board->getGameState() << endl;
}
void Engine::doMove()
{
	string coords, start, end, moveStatus;
	cin >> coords;

	int tmp = (int)coords.find('-');
	start = coords.substr(0, tmp);
	end = coords.substr(tmp + 1, coords.size());

	moveStatus = board->checkMove(start, end,false);
	cout << moveStatus << endl << endl;
}

void Engine::allMoves(bool justNumber, bool justWinning, bool justUpdateGameState)
{
	unordered_map<string, vector<vector<char>>>* uniqueMaps = new unordered_map<string, vector<vector<char>>>;
	if (board != nullptr) {
		board->getAllMoves(uniqueMaps, justWinning);
		if (justUpdateGameState){
			return; //just to update gameState
		}
		if (justNumber) {
			cout << uniqueMaps->size() << UNIQUE_MOVES_NUMBER << endl;
			return;
		}
		for (const auto& tempPair : *uniqueMaps) {
			cout << tempPair.first << endl;
		}
	}
}