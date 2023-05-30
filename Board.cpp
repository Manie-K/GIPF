#include "Board.h"

Board::Board(int S, int K, int GW, int GB) :size(S), maxSize(2 * S - 1), pieceCollectSize(K), outsideSize(S + 1), totalWhite(GW),
totalBlack(GB), boardStatus(""), gameState(GAME_STATE_PROGRESS)
{
	string whiteP, blackP;
	int wh, bl;
	char cur;

	cin >> whiteP >> blackP >> cur;
	wh = stringToInt(whiteP);
	bl = stringToInt(blackP);

	players = new Players(wh, bl, cur);
	load(GW, wh, GB, bl);
}
Board::~Board()
{
	delete players;
}

string Board::getBoardStatus()const { return boardStatus; }
string Board::getGameState() const { return gameState; }

void Board::print() const
{
	int whiteReserve = players->getCurrent()->getColor() == WHITE_PIECE ?
		players->getCurrent()->getPieces() : players->getOpponent()->getPieces();
	int blackReserve = players->getCurrent()->getColor() == BLACK_PIECE ?
		players->getCurrent()->getPieces() : players->getOpponent()->getPieces();

	cout << size << ' ' << pieceCollectSize << ' ' << totalWhite << ' ' << totalBlack << '\n';
	cout << whiteReserve << ' ' << blackReserve << ' ' << players->getCurrent()->getColor() << '\n';

	for (int y = 1; y <= size; y++)
	{
		for (int z = 0; z < (int)maxSize - size - (y - 1); z++)
		{
			cout << ' ';
		}
		for (int x = 1; x < (int)map[y].size() - 1; x++)
		{
			cout << map[y][x] << ' ';
		}
		cout << '\n';
	}
	for (int y = size + 1; y <= maxSize; y++)
	{
		for (int z = 1; z <= y - size; z++)
		{
			cout << ' ';
		}
		for (int x = 1; x < (int)map[y].size() - 1; x++)
		{
			cout << map[y][x] << ' ';
		}
		cout << '\n';
	}
}

string Board::checkMove(const string& start, const string& end)
{
	pair<int, int> startPos = getPosByName(start), endPos = getPosByName(end);
	vector<pair<int, int>> line = getLine(start, end);

	if (startPos.first == -1)
		return MOVE_STATUS_BAD_PREFIX + start + MOVE_STATUS_INDEX;
	if (endPos.first == -1)
		return MOVE_STATUS_BAD_PREFIX + end + MOVE_STATUS_INDEX;
	if (line.empty())
		return MOVE_STATUS_DIR;
	if (map.at(startPos.second).at(startPos.first) != OUTSIDE_PIECE)
		return MOVE_STATUS_BAD_PREFIX + start + MOVE_STATUS_BAD_START;
	if (map.at(endPos.second).at(endPos.first) == OUTSIDE_PIECE)
		return MOVE_STATUS_BAD_PREFIX + end + MOVE_STATUS_BAD_DEST;
	bool emptyPlace = false;
	for (auto pos : line)
	{
		if (map.at(pos.second).at(pos.first) == EMPTY_PIECE) {
			emptyPlace = true;
			break;
		}
	}
	if (!emptyPlace)
		return MOVE_STATUS_ROW;
	if (move(line, endPos))
		return MOVE_STATUS_OK;
	return "";
}
bool Board::move(vector<pair<int, int>>& line, const pair<int, int>& endPos)
{
	vector<vector<char>> tempMap;
	for (vector<char> innerVector : map) {
		tempMap.push_back(innerVector);
	}
	const char color = players->getCurrent()->getColor();
	int i = 0;
	while (i < (int)line.size() - 1)
	{
		if (map.at(line.at(i).second).at(line.at(i).first) == EMPTY_PIECE)
			break;
		i++;
	}
	for (; i > 0; i--)
	{
		map.at(line.at(i).second).at(line.at(i).first) = map.at(line.at(i - 1).second).at(line.at(i - 1).first);
	}
	map.at(endPos.second).at(endPos.first) = color;

	vector<vector<pair<int, int>>>* nonCollidingChains = checkForChains();
	vector<vector<pair<int, int>>>* collidingChains = setCollidingChains(nonCollidingChains);

	if (collidingChains->size() > 0)
	{
		const char whiteLower = 'w', blackLower = 'b';
		bool removal = false;
		char chainColor;
		string c, start, end;
		cin >> c >> start >> end;

		pair<int, int> s = getPosByName(start), e = getPosByName(end);
		chainColor = c.at(0) == whiteLower ? WHITE_PIECE : BLACK_PIECE;

		for (vector<pair<int, int>>& chain : *collidingChains)
		{
			if (s.first == -1) break;
			if (thisIsChosenLine(chain, s, e))
			{
				if (checkChainColor(chain) != chainColor)
				{
					cout << MOVE_CHOICE_WRONG_COLOR << endl;
					gameState = GAME_STATE_BAD_MOVE;
					map = tempMap;
					delete nonCollidingChains;
					delete collidingChains;
					return false;
				}
				removeGivenChain(chain);
				removal = true;
			}
		}
		if (!removal || s.first == -1 || e.first == -1) {
			cout << MOVE_CHOICE_WRONG_INDEX << endl;
			gameState = GAME_STATE_BAD_MOVE;
			map = tempMap;
			delete nonCollidingChains;
			delete collidingChains;
			return false;
		}
	}

	for (vector<pair<int, int>>& chain : *nonCollidingChains)
	{
		removeGivenChain(chain);
	}

	players->getCurrent()->setPieces(players->getCurrent()->getPieces() - 1);
	players->switchPlayers();

	if (players->getCurrent()->getPieces() <= 0)
	{
		if (players->getOpponent()->getColor() == WHITE_PIECE)
			gameState = GAME_STATE_WHITE_WIN;
		if (players->getOpponent()->getColor() == BLACK_PIECE)
			gameState = GAME_STATE_BLACK_WIN;
	}
	else {
		gameState = GAME_STATE_PROGRESS;
	}

	delete nonCollidingChains;
	delete collidingChains;
	return true;
}

void Board::load(const int whiteMax, int whiteReserve, const int blackMax, int blackReserve)
{
	string row;
	int whiteOnMap = 0, blackOnMap = 0;
	vector<string> charMap;

	for (int i = 0; i <= maxSize; i++) //first line is ""
	{
		getline(cin, row);
		if (row != "")
			charMap.push_back(row);
		whiteOnMap += (int)count(row.begin(), row.end(), WHITE_PIECE);
		blackOnMap += (int)count(row.begin(), row.end(), BLACK_PIECE);
	}

	for (int y = 0; y < maxSize; y++)
	{
		vector<char> temp;
		map.push_back(temp);
		int rowSize = (int)charMap[y].size();
		for (int x = 0; x < rowSize; x++)
		{
			if (charMap[y][x] != ' ')
			{
				map[y].push_back(charMap[y][x]);
			}
		}
	}

	//check for size
	for (int y = 0; y < size; y++)
	{
		if ((int)map[y].size() != size + y || (int)map[maxSize - y - 1].size() != size + y)
		{
			boardStatus = BOARD_STATUS_SIZE;
			return;
		}
	}
	//check other
	if (whiteOnMap + whiteReserve > whiteMax) {
		boardStatus = BOARD_STATUS_WHITE_COUNT;
		return;
	}
	else if (blackOnMap + blackReserve > blackMax) {
		boardStatus = BOARD_STATUS_BLACK_COUNT;
		return;
	}
	else
		boardStatus = BOARD_STATUS_OK;
	//add side tiles
	for (int y = 0; y < maxSize; y++)
	{
		map[y].push_back(OUTSIDE_PIECE);
		map[y].insert(map[y].begin(), OUTSIDE_PIECE);
	}
	//add top and bottom tiles
	vector<char> temp1, temp2;
	for (int i = 0; i <= size; i++)
	{
		temp1.push_back(OUTSIDE_PIECE);
		temp2.push_back(OUTSIDE_PIECE);
	}
	map.push_back(temp2);
	map.insert(map.begin(), temp1);
	//map ready
	loadHashMap();

	//checking if chains are removed
	int chainsNumber = (int)checkForChains()->size();
	if (chainsNumber > 0) {
		boardStatus = BOARD_STATUS_ERROR + to_string(chainsNumber);
		if (chainsNumber == 1)
			boardStatus += BOARD_STATUS_CHAIN_SINGLE;
		else
			boardStatus += BOARD_STATUS_CHAINS_MULTIPLE;
	}
}
void Board::loadHashMap()
{
	const int letterCount = 2 * (outsideSize)-1;
	const char baseLetter = 'a';

	int range = outsideSize;
	int* lettersLeft = new int[letterCount];
	//setting up helping array
	for (int i = 0; i < letterCount; i++)
	{
		lettersLeft[i] = range;
		if (i < letterCount / 2)
			range++;
		else
			range--;
	}
	//initializing hash map 
	for (int y = 0; y < 2 * outsideSize - 1; y++)
	{
		int rowSize = (int)map[y].size();
		char letter = baseLetter;
		for (int x = 0; x < rowSize; x++)
		{
			string key = "";
			int tempIndex = (int)(letter - baseLetter);
			while (tempIndex < letterCount && lettersLeft[tempIndex] <= 0)
			{
				tempIndex++;
				letter++;
			}
			key += letter;
			char tempNumber = (char)lettersLeft[tempIndex] + '0';
			key += tempNumber;
			lettersLeft[tempIndex]--;
			posMap.insert({ key,make_pair(x, y) });
			nameMap.insert({ makeStringFromPos(make_pair(x, y)) ,key });
			letter++;
		}
	}
	delete[] lettersLeft;
}

string Board::getNameByPos(const pair<int, int>& pos) const
{
	try {
		string retString = nameMap.at(makeStringFromPos(pos));
		return retString;
	}
	catch (const out_of_range& e)
	{
		cerr << "Exception caught: " << e.what() << '\n';
		cerr << "Accesing wrong position: (" << pos.first << ", " << pos.second << ")!\n";
		return "";
	}
}
pair<int, int> Board::getPosByName(const string& name) const
{
	if (posMap.find(name) == posMap.end())
	{
		return make_pair<int, int>(-1, -1);
	}
	return posMap.at(name);
}

vector<pair<int, int>> Board::getLine(const string& nameA, const string& nameB) const
{
	vector<pair<int, int>> retVector;
	pair<int, int> tempPair;
	string tempStr = "";
	int dN = 0;
	int a = 0, b = 0;
	char baseLetter = nameB.at(0);
	const char dL = baseLetter - nameA.at(0);
	const char middleLetter = (((2 * (char)outsideSize) - 1) / 2) + 'a';

	retVector.push_back(getPosByName(nameA));
	retVector.push_back(getPosByName(nameB));

	if (nameA.size() > 2 || nameB.size() > 2)
	{
		for (int i = 1; i < (int)nameA.size(); i++)
		{
			a = a * 10 + nameA.at(i);
		}
		for (int i = 1; i < (int)nameB.size(); i++)
		{
			b = b * 10 + nameB.at(i);
		}
	}
	else
	{
		a = nameA.at(1);
		b = nameB.at(1);
	}
	dN = b - a;

	//6 cases
	if (dL == 0 && (dN == 1 || dN == -1))
	{
		tempPair = getPosByName(nameB);
		while (tempPair.first != -1)
		{
			b += dN;
			tempStr = makeStringFromName(b, baseLetter);
			tempPair = getPosByName(tempStr);
			if (tempPair.first != -1)
				retVector.push_back(tempPair);
		}
	}
	else if (dL == 1 && dN == 1)
	{
		tempPair = getPosByName(nameB);
		while (tempPair.first != -1)
		{
			if (baseLetter < middleLetter)
				b += dN;
			baseLetter += dL;
			tempStr = makeStringFromName(b, baseLetter);
			tempPair = getPosByName(tempStr);
			if (tempPair.first != -1)
				retVector.push_back(tempPair);
		}
	}
	else if (dL == 1 && dN == 0)
	{
		tempPair = getPosByName(nameB);
		while (tempPair.first != -1)
		{
			baseLetter += dL;
			if (baseLetter > middleLetter)
				b -= 1;
			tempStr = makeStringFromName(b, baseLetter);
			tempPair = getPosByName(tempStr);
			if (tempPair.first != -1)
				retVector.push_back(tempPair);
		}
	}
	else if (dL == -1 && dN == 0)
	{
		tempPair = getPosByName(nameB);
		while (tempPair.first != -1)
		{
			baseLetter += dL;
			if (baseLetter < middleLetter)
				b -= 1;
			tempStr = makeStringFromName(b, baseLetter);
			tempPair = getPosByName(tempStr);
			if (tempPair.first != -1)
				retVector.push_back(tempPair);
		}
	}
	else if (dL == -1 && dN == 1)
	{
		tempPair = getPosByName(nameB);
		while (tempPair.first != -1)
		{
			if (baseLetter > middleLetter)
				b += dN;
			baseLetter += dL;
			tempStr = makeStringFromName(b, baseLetter);
			tempPair = getPosByName(tempStr);
			if (tempPair.first != -1)
				retVector.push_back(tempPair);
		}
	}
	else retVector.clear();

	return retVector;
}

void Board::removeGivenChain(const vector<pair<int, int>>& chain)
{
	int white = 0, black = 0;
	int whiteStreak = 0, blackStreak = 0;
	bool wS = false, bS = false;

	for (int i = 0; i < (int)chain.size(); i++)
	{
		if (map.at(chain.at(i).second).at(chain.at(i).first) == WHITE_PIECE) {
			white++;
			whiteStreak++;
			blackStreak = 0;
		}
		else if (map.at(chain.at(i).second).at(chain.at(i).first) == BLACK_PIECE) {
			black++;
			blackStreak++;
			whiteStreak = 0;
		}
		map.at(chain.at(i).second).at(chain.at(i).first) = EMPTY_PIECE;
		if (whiteStreak >= pieceCollectSize)
			wS = true;
		else if (blackStreak >= pieceCollectSize)
			bS = true;
	}
	//handle player pieces
	if (players->getCurrent()->getColor() == WHITE_PIECE)//current white
	{
		if (wS)
		{
			players->getCurrent()->setPieces(players->getCurrent()->getPieces() + white);
		}
		else if (bS)
		{
			players->getOpponent()->setPieces(players->getOpponent()->getPieces() + black);
		}
	}
	else //current black
	{
		if (wS)
		{
			players->getOpponent()->setPieces(players->getOpponent()->getPieces() + white);
		}
		else if (bS)
		{
			players->getCurrent()->setPieces(players->getCurrent()->getPieces() + black);
		}
	}

}

void Board::chainsInLine(const vector<pair<int, int>>& line, vector<vector<pair<int, int>>>* chains) const
{
	bool chainFlag = false;
	int whiteStreak = 0, blackStreak = 0;
	vector<pair<int, int>> tempChain;
	for (int i = 0; i < (int)line.size(); i++)
	{

		if (map.at(line.at(i).second).at(line.at(i).first) == WHITE_PIECE)
		{
			whiteStreak++;
			blackStreak = 0;
			tempChain.push_back(line.at(i));
		}
		else if (map.at(line.at(i).second).at(line.at(i).first) == BLACK_PIECE)
		{
			blackStreak++;
			whiteStreak = 0;
			tempChain.push_back(line.at(i));
		}
		else
		{
			if (chainFlag)
				chains->push_back(tempChain);
			tempChain.clear();
			blackStreak = whiteStreak = 0;
			chainFlag = false;
		}
		if (whiteStreak >= pieceCollectSize || blackStreak >= pieceCollectSize)
		{
			chainFlag = true;
		}
	}
}
vector<vector<pair<int, int>>>* Board::checkForChains() const
{
	vector<vector<pair<int, int>>>* chains = new vector<vector<pair<int, int>>>;
	const char baseLetter = 'a';
	char letter = baseLetter;
	int number = 1;
	string tempStrA = "", tempStrB = "";
	vector<pair<int, int>> lineToCheck;

	//horizontal upper
	for (int i = 0; i < size; i++)
	{
		tempStrA = makeStringFromName(number, letter);
		tempStrB = makeStringFromName(number + 1, letter + 1);
		number++;
		lineToCheck = getLine(tempStrA, tempStrB);
		chainsInLine(lineToCheck, chains);
	}
	//horizontal lower
	number = 1;
	letter = baseLetter + 1;
	for (int i = 0; i < size - 1; i++)
	{
		tempStrA = makeStringFromName(number, letter);
		tempStrB = makeStringFromName(number + 1, letter + 1);
		letter++;
		lineToCheck = getLine(tempStrA, tempStrB);
		chainsInLine(lineToCheck, chains);
	}
	//top left to bottom right
	number = outsideSize;
	letter = baseLetter;
	for (int i = 0; i < size; i++)
	{
		tempStrA = makeStringFromName(number, letter);
		tempStrB = makeStringFromName(number, letter + 1);
		letter++;
		number++;
		lineToCheck = getLine(tempStrA, tempStrB);
		chainsInLine(lineToCheck, chains);
	}

	//top right to bottom left
	number = 2 * outsideSize - 1;
	letter = baseLetter + (char)size;
	for (int i = 0; i < size; i++)
	{
		tempStrA = makeStringFromName(number, letter);
		tempStrB = makeStringFromName(number - 1, letter);
		letter--;
		number--;
		lineToCheck = getLine(tempStrA, tempStrB);
		chainsInLine(lineToCheck, chains);
	}

	//top left to bottom right, but lower
	number = size;
	letter = baseLetter;
	for (int i = 0; i < size - 1; i++)
	{
		tempStrA = makeStringFromName(number, letter);
		tempStrB = makeStringFromName(number, letter + 1);
		number--;
		lineToCheck = getLine(tempStrA, tempStrB);
		chainsInLine(lineToCheck, chains);
	}

	//top right to bottom left, but lower
	number = 2 * size;
	letter = baseLetter + (char)size + 1;
	for (int i = 0; i < size - 1; i++)
	{
		tempStrA = makeStringFromName(number, letter);
		tempStrB = makeStringFromName(number - 1, letter);
		letter++;
		number--;
		lineToCheck = getLine(tempStrA, tempStrB);
		chainsInLine(lineToCheck, chains);
	}

	return chains;
}
vector<vector<pair<int, int>>>* Board::setCollidingChains(vector<vector<pair<int, int>>>*& all) const
{
	vector<vector<pair<int, int>>>* retVector = new vector<vector<pair<int, int>>>;
	for (vector<pair<int, int>>& line : *all)
	{
		for (pair<int, int>& coords : line)
		{
			int count = getCoordsCount(coords, all);
			if (count != 1)
			{
				retVector->push_back(line);
			}
		}
	}
	//delete colliding chains from all chains
	vector<vector<pair<int, int>>>* tempVector = new vector<vector<pair<int, int>>>;

	sort(all->begin(), all->end());
	sort(retVector->begin(), retVector->end());
	set_difference(all->begin(), all->end(), retVector->begin(), retVector->end(), back_inserter(*tempVector));

	delete all;
	all = tempVector;
	return retVector;
}
char Board::checkChainColor(const vector<pair<int, int>>& line) const
{
	int white = 0, black = 0;
	for (auto& point : line)
	{
		if (map.at(point.second).at(point.first) == WHITE_PIECE)
			white++;
		else if (map.at(point.second).at(point.first) == BLACK_PIECE)
			black++;
	}

	if (white > black) //they cant be equal 
		return WHITE_PIECE;
	return BLACK_PIECE;
}

bool Board::thisIsChosenLine(const vector<pair<int, int>>& line, const pair<int, int>& start, const pair<int, int>& end)
{
	pair<int, int> pos;
	int startIndex = -1, endIndex = -1;
	const char color = map.at(start.second).at(start.first);

	for (int i = 0; i < (int)line.size(); i++)
	{
		if (line.at(i) == start)
			startIndex = i;
		if (line.at(i) == end)
			endIndex = i;
	}
	if (startIndex == -1 || endIndex == -1)
		return false;

	const int delta = startIndex < endIndex ? 1 : -1;
	for (int i = startIndex; i != endIndex; i += delta)
	{
		pos = line.at(i);
		if (map.at(pos.second).at(pos.first) != color)
			return false;
	}

	if (startIndex > 0) {
		pos = line.at(startIndex - 1);
		if (map.at(pos.second).at(pos.first) == color && startIndex < (int)line.size() - 1) {
			pos = line.at(startIndex + 1);
			if (map.at(pos.second).at(pos.first) == color)
				return false;
		}
	}
	if (endIndex > 0) {
		pos = line.at(endIndex - 1);
		if (map.at(pos.second).at(pos.first) == color && endIndex < (int)line.size() - 1)
		{
			pos = line.at(endIndex + 1);
			if (map.at(pos.second).at(pos.first) == color)
				return false;
		}
	}
	return true;
}

int Board::getCoordsCount(pair<int, int> p, vector<vector<pair<int, int>>>* set)
{
	int count = 0;
	for (vector<pair<int, int>>& line : *set)
	{
		for (pair<int, int>& coords : line)
		{
			if (coords == p)
				count++;
		}
	}
	return count;
}
string Board::makeStringFromPos(const pair<int, int>& pos)
{
	string retString = "";
	int x = pos.first, y = pos.second;
	retString += to_string(x);
	retString += '_';
	retString += to_string(y);
	return retString;
}
string Board::makeStringFromName(int num, char letter)
{
	string tempStr = "";
	tempStr += letter;

	const int min = 48; //when grater than 57('9') then it got increased beyond 9 in code, and we want to return for example 10
	if (num >= min)
		num -= '0';
	string tempInt = to_string(num);
	tempStr += tempInt;


	return tempStr;
}

int Board::stringToInt(const string& str)
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

	while (i < (int)str.length()) {
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