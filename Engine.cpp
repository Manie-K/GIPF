#include "Engine.h"

Engine::Engine() :board(nullptr), command(""), gameState(GAME_STATE_PROGRESS), exit(false) {}
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
	else if (command == GAME_STATE_CMD)
		printGameState();
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
	cout << boardStatus << endl << endl;
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
	cout << board->getGameState() << endl << endl;
}
void Engine::doMove()
{
	string coords, start, end, moveStatus;
	cin >> coords;

	int tmp = (int)coords.find('-');
	start = coords.substr(0, tmp);
	end = coords.substr(tmp + 1, coords.size());

	moveStatus = board->checkMove(start, end);
	cout << moveStatus << endl << endl;
}