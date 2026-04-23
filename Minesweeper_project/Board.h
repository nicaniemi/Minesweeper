#pragma once
#include<vector>
using namespace std;

struct Tile
{
	bool mine;
	bool flagged;
	bool revealed;
	int neighborMines;
	vector<Tile*> adjacentTiles;
};

class Board
{
public:
	Board();
	const int getWidth();
	const int getHeight();
	const vector<vector<Tile>>& getTileVector();
	void flagTile(int row, int col);
	void revealTile(Tile* tile);
	void revealTile(int row, int col);
	bool isGameOver();
	bool isGameWon();
	void build(string filename = "");
	int getFlaggedTiles();
	int getNumMines();
private:
	int width;
	int height;
	int numMines;
	int flaggedTiles;
	vector<vector<Tile>> tileVector;
	bool gameOver;
	bool gameWon;
};
