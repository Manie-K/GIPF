#pragma once
#include <vector>
#include <string>
#include <list>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include "Players.h"

using namespace std;

class Engine;

class Board
{
private:
	const int size, maxSize, pieceCollectSize, outsideSize, totalWhite, totalBlack;
	Players* players;
	string boardStatus, gameState;
	vector<vector<char>> map;
	unordered_map<string, pair<int, int>> posMap;
	unordered_map<string, string> nameMap; //first string is position written in string
public:
	Board(int S, int K, int GW, int GB);
	~Board();
	
	string getBoardStatus()const;
	string getGameState() const;
	
	void print() const;
	
	string checkMove(const string& start, const string& end, bool justWinning, bool* gameWon = nullptr, unordered_map<string, vector<vector<char>>>* uniqueMaps = nullptr);
	void getAllMoves(unordered_map<string, vector<vector<char>>>*& uniqueMaps, bool justWinning);

	static int stringToInt(const string& str);
private:
	void load(const int whiteMax, int whiteReserve, const int blackMax, int blackReserve);
	void loadHashMap();
	bool isBoardOkay(int whiteRes, int blackRes, int whiteOnMap, int whiteMax, int blackOnMap, int blackMax);
	void addOutsideOfMap();
	void areChainsRemovedOnLoad();

	string getNameByPos(const pair<int, int>& pos) const;
	pair<int, int> getPosByName(const string& name) const;
	
	bool move(vector<pair<int, int>>& line, const pair<int, int>& endPos, 
		string& key, bool justWinning, bool* gameWon=nullptr, unordered_map<string, vector<vector<char>>>* uniqueMaps = nullptr);
	bool checkIfIsUniqueMap(unordered_map<string, vector<vector<char>>>*& uniqueMaps,const  vector<vector<char>> tempMap) const;

	vector<pair<int, int>> getLine(const string& nameA, const string& nameB) const;

	string removeGivenChain(const vector<pair<int, int>>& chain);
	bool handleCollidingChains(vector<vector<pair<int, int>>>* collidingChains);

	void chainsInLine(const vector<pair<int, int>>& line, vector<vector<pair<int, int>>>* chains) const;
	vector<vector<pair<int, int>>>* checkForChains() const;
	vector<vector<pair<int, int>>>* setCollidingChains(vector<vector<pair<int, int>>>*& all) const;
	
	char checkChainColor(const vector<pair<int, int>>& line) const;
	bool thisIsChosenLine(const vector<pair<int, int>>& line, const pair<int, int>& start, const pair<int, int>& end);

	static int getCoordsCount(pair<int, int> p, vector<vector<pair<int, int>>>* set);
	static string makeStringFromPos(const pair<int, int>& pos);
	static string makeStringFromName(int num, char letter);
};