#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include "Players.h"
#include "Tile.h"

using namespace std;

class Board
{
public:

private:
	const int size, maxSize, pieceCollectSize;
	Players* players;
	string boardStatus;
	vector<string> charMap;
	vector<vector<Tile>> map;
	unordered_map<string, pair<int, int>> hashMap;
public:
	Board(int S, int K, int GW, int GB):size(S),maxSize(2 * S - 1),pieceCollectSize(K)
	{
		string whiteP, blackP;
		char cur;
		cin >> whiteP >> blackP >> cur;
		players = new Players(stoi(whiteP), stoi(blackP), cur);
		load(GW, stoi(whiteP),GB, stoi(blackP));
		//loadHashMap();
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
		int charMapIndex = 0;

		for(int i = -1; i < maxSize;i++) //first line is ""
		{
			getline(cin, row);
			if(row!="")
				charMap.push_back(row);
			whiteOnMap += count(row.begin(), row.end(), WHITE_PIECE);
			blackOnMap += count(row.begin(), row.end(), BLACK_PIECE);
		}

		for (int y = 0; y < maxSize+2; y++)
		{
			int rowSize = charMap[y].size();
			for (int x = 0; x < rowSize; x++)
			{
				if (charMap[y][x] != ' ')
				{
					Tile tempTile(x, y, charMap[y][x]);
					//set letter and number
					//add to hashMap
					map[y].push_back(tempTile);
				}
			}
		}

		//check for size

		//add side tiles
		for (int y = 0; y < maxSize; y++)
		{

		}
		//add top and bottom tiles


		if (whiteOnMap + whiteReserve != whiteMax)
			boardStatus = BOARD_STATUS_WHITE_COUNT;
		else if (blackOnMap + blackReserve != blackMax)
			boardStatus = BOARD_STATUS_BLACK_COUNT;
		else
			boardStatus = BOARD_STATUS_OK;
	}
	void loadHashMap()
	{

	}
};