#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include "Players.h"

using namespace std;

class Board
{
public:

private:
	const int size, maxSize, pieceCollectSize, outsideSize;
	Players* players;
	string boardStatus;
	vector<string> charMap;
	vector<vector<char>> map;
	unordered_map<string, pair<int, int>> hashMap;
public:
	Board(int S, int K, int GW, int GB):size(S),maxSize(2 * S - 1),pieceCollectSize(K),outsideSize(S+1)
	{
		string whiteP, blackP;
		char cur;
		cin >> whiteP >> blackP >> cur;
		players = new Players(stoi(whiteP), stoi(blackP), cur);
		load(GW, stoi(whiteP),GB, stoi(blackP));
	}
	~Board()
	{
		delete players;
	}
	string getBoardStatus()const { return boardStatus; }
	void print() const
	{
		for (string row : charMap)
		{
			cout << row << '\n';
		}
	}
private:
	void load(const int whiteMax,int whiteReserve, const int blackMax, int blackReserve)
	{
		string row;
		int whiteOnMap = 0, blackOnMap = 0;

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
		for (int y = 0; y < outsideSize; y++)
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
				hashMap[key] = make_pair(x, y);
				letter++;
			}
		}
		delete[] lettersLeft;
	}
};