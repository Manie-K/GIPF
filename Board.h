#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include "Players.h"
#include "Engine.h"

using namespace std;

class Engine;

class Board
{
public:

private:
	const int size, maxSize, pieceCollectSize, outsideSize, totalWhite, totalBlack;
	Players* players;
	string boardStatus;
	vector<vector<char>> map;
	unordered_map<string, pair<int, int>> posMap;
	unordered_map<string, string> nameMap; //first string is position written in string

public:
	Board(int S, int K, int GW, int GB):size(S),maxSize(2 * S - 1),pieceCollectSize(K),outsideSize(S+1),totalWhite(GW),
		totalBlack(GW)
	{
		string whiteP, blackP;
		int wh, bl;
		char cur;
		
		cin >> whiteP >> blackP >> cur;
		wh = stringToInt(whiteP);
		bl = stringToInt(blackP);

		players = new Players(wh,bl, cur);
		load(GW, wh,GB, bl);
	}
	~Board()
	{
		delete players;
	}
	string getBoardStatus()const { return boardStatus; }
	void print() const
	{
		int whiteReserve = players->getCurrent()->getColor() == WHITE_PIECE ?
			players->getCurrent()->getPieces() : players->getOpponent()->getPieces();
		int blackReserve = players->getCurrent()->getColor() == BLACK_PIECE ?
			players->getCurrent()->getPieces() : players->getOpponent()->getPieces();

		cout << size << ' ' << pieceCollectSize << ' ' << totalWhite << ' ' << totalBlack << '\n';
		cout << whiteReserve << ' ' << blackReserve << ' ' << players->getCurrent()->getColor() << '\n';

		for (int y = 1; y <= size; y++)
		{
			for (int z = 0; z < maxSize-size-(y-1); z++)
			{
				cout << ' ';
			}
			for (int x = 1; x < map[y].size()-1; x++)
			{
				cout << map[y][x] << ' ';
			}
			cout << '\n';
		}
		for (int y = size+1 ; y <= maxSize; y++)
		{
			for (int z = 1; z <= y-size; z++)
			{
				cout << ' ';
			}
			for (int x = 1; x < map[y].size() - 1; x++)
			{
				cout << map[y][x] << ' ';
			}
			cout << '\n';
		}
	}
	string checkMove(const string& start, const string& end)
	{
		pair<int, int> startPos = getPosByName(start), endPos = getPosByName(end);
		vector<pair<int, int>> line = getLine(start, end);

		if (startPos.first == -1)
			return MOVE_STATUS_INDEX(start);
		if (endPos.first == -1)
			return MOVE_STATUS_INDEX(end);
		if (line.empty())
			return MOVE_STATUS_DIR;
		if (map.at(startPos.second).at(startPos.first) != OUTSIDE_PIECE)
			return MOVE_STATUS_BAD_START(start);
		if(map.at(endPos.second).at(endPos.first) == OUTSIDE_PIECE)
			return MOVE_STATUS_BAD_DEST(start);
		bool emptyPlace = false;
		for (auto pos : line) 
		{
			if (map.at(pos.second).at(pos.first) == EMPTY_PIECE) {
				emptyPlace = true;
				break;
			}
		}
		pair<int, int> lastBoardPos = line.at(line.size()-2);
		if(!emptyPlace || map.at(lastBoardPos.second).at(lastBoardPos.first)!=EMPTY_PIECE)
			return MOVE_STATUS_ROW;
		move(line, endPos);
		return MOVE_STATUS_OK;
	}
	void move(vector<pair<int, int>>& line, const pair<int, int>& endPos)
	{
		const char color = players->getCurrent()->getColor();
		for (int i = line.size()-1; i > 0; i--)
		{
			map.at(line.at(i).second).at(line.at(i).first) = map.at(line.at(i - 1).second).at(line.at(i - 1).first);
		}
		map.at(endPos.second).at(endPos.first) = color;

		players->getCurrent()->setPieces(players->getCurrent()->getPieces()-1);
		players->switchPlayers();
	}
private:
	void load(const int whiteMax,int whiteReserve, const int blackMax, int blackReserve)
	{
		string row;
		int whiteOnMap = 0, blackOnMap = 0;
		vector<string> charMap;

		for(int i = 0; i <= maxSize;i++) //first line is ""
		{
			getline(cin, row);
			if(row!="")
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
			if (map[y].size() != size + y || map[maxSize - y - 1].size() != size + y)
			{
				boardStatus = BOARD_STATUS_SIZE;
				return;
			}
		}
		//check other
		if (whiteOnMap + whiteReserve != whiteMax) {
			boardStatus = BOARD_STATUS_WHITE_COUNT;
			return;
		}
		else if (blackOnMap + blackReserve != blackMax) {
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
		vector<char> temp1,temp2;
		for (int i = 0; i <= size; i++)
		{
			temp1.push_back(OUTSIDE_PIECE);
			temp2.push_back(OUTSIDE_PIECE);
		}
		map.push_back(temp2);
		map.insert(map.begin(), temp1);
		//map ready
		loadHashMap();
	}
	void loadHashMap()
	{
		const int letterCount = 2*(outsideSize)-1;
		const char baseLetter = 'a';

		int range = outsideSize;
		int* lettersLeft = new int[letterCount];
		//setting up helping array
		for (int i = 0; i < letterCount; i++)
		{
			lettersLeft[i] = range;
			if (i < letterCount/2)
				range++;
			else
				range--;
		}
		//initializing hash map 
		for (int y = 0; y < 2*outsideSize-1; y++)
		{
			int rowSize = (int)map[y].size();
			char letter = baseLetter;
			for (int x = 0; x < rowSize; x++)
			{
				string key = "";
				int tempIndex = (int)(letter - baseLetter);
				while(tempIndex < letterCount && lettersLeft[tempIndex]<=0)
				{
					tempIndex++;
					letter++;
				}
				key += letter;
				char tempNumber = (char)lettersLeft[tempIndex]+'0';
				key += tempNumber;
				lettersLeft[tempIndex]--;
				posMap.insert({ key,make_pair(x, y) });
				nameMap.insert({makeStringFromPos(make_pair(x, y)) ,key});
				letter++;
			}
		}
		delete[] lettersLeft;
	}
	string getNameByPos(const pair<int, int>& pos) const  
	{
		try {
			string retString = nameMap.at(makeStringFromPos(pos));
			return retString;
		}
		catch (const out_of_range& e)
		{
			cerr << "Exception caught: " << e.what() << '\n';
			cerr << "Accesing wrong position: (" << pos.first << ", " << pos.second << ")!\n";
		}
	}
	pair<int, int> getPosByName(const string& name) const 
	{
		try {
			pair<int,int> retPair = posMap.at(name);
			return retPair;
		}
		catch (const out_of_range& e)
		{
			return make_pair<int,int>(-1, -1);
			//cerr << "Exception caught: " << e.what() << '\n';
			//cerr << "Accesing wrong name: ("<< name<< ")!\n";
		}
	}
	static string makeStringFromPos(const pair<int, int>& pos)
	{
		string retString = "";
		int x = pos.first, y = pos.second;
		retString += to_string(x);
		retString += '_';
		retString += to_string(y);
		return retString;
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
	vector<pair<int,int>> getLine(const string& nameA, const string& nameB) const
	{
		vector<pair<int, int>> retVector;
		pair<int, int> tempPair;
		string tempStr = "";
		int dN = 0;
		int a = 0, b = 0;
		int baseLetter = nameB.at(0);
		const int dL = baseLetter - nameA.at(0);
		const char middleLetter = (((2 * outsideSize) - 1)/2)+'a';

		retVector.push_back(getPosByName(nameA));
		retVector.push_back(getPosByName(nameB));

		if (nameA.size() > 2 || nameB.size() > 2)
		{
			for (int i = 1; i < nameA.size(); i++)
			{
				a = a * 10 + nameA.at(i);
			}
			for (int i = 1; i < nameB.size(); i++)
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
				baseLetter += dL;
				if (baseLetter < middleLetter)
					b += dN;
				tempStr = makeStringFromName(b, baseLetter);
				tempPair = getPosByName(tempStr);
				if (tempPair.first != -1)
					retVector.push_back(tempPair);
			}
		}
		else if ((dL == 1 || dL == -1) && dN == 0)
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
		else if (dL == -1 && dN == 1)
		{
			tempPair = getPosByName(nameB);
			while (tempPair.first != -1)
			{
				baseLetter += dL;
				if (baseLetter < middleLetter)
					b += dN;
				tempStr = makeStringFromName(b, baseLetter);
				tempPair = getPosByName(tempStr);
				if(tempPair.first != -1)
					retVector.push_back(tempPair);
			}
		}
		else retVector.clear();

		return retVector;
	}

	static string makeStringFromName(int num, char letter)
	{
		string tempStr = "";
		tempStr += letter;
		tempStr += num;
		return tempStr;
	}
};