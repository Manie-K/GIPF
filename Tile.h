#pragma once

class Tile
{
public:
private:
	bool onMap;
	pair<int, int> positionInVector;
	char letter;
	char number;
	char color;
public:
	Tile(int x, int y, char color):positionInVector(make_pair(x,y)),color(color)
	{
		
	}
	~Tile(){}
private:

};
