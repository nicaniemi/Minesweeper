#include <SFML/Graphics.hpp>
#include<iostream>
#include<iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include<algorithm>
#include "Board.h"
using namespace std;

void Board::build(string filename)
{
	gameOver = false;
	flaggedTiles = 0;
	tileVector.clear();

	if (filename == "")
	{
		ifstream file("boards/config.cfg");
		string singleLine;

		getline(file, singleLine);
		width = stoi(singleLine); // Columns

		getline(file, singleLine);
		height = stoi(singleLine); // Rows

		getline(file, singleLine);
		numMines = stoi(singleLine); // Mines

		tileVector.resize(height); // Creates a vector of all tiles in the game
		for (unsigned int i = 0; i < height; i++)
		{
			tileVector[i].resize(width);
		}

		vector<pair<int, int>> combos; // A vector of all possible ordered pairs (row, column)

		for (unsigned int i = 0; i < height; i++)
		{
			for (unsigned int j = 0; j < width; j++)
			{
				combos.push_back(make_pair(i, j));
			}
		}

		random_device mt;
		mt19937 random_mt(mt());
		shuffle(combos.begin(), combos.end(), random_mt); // Shuffle the ordered pairs and assign 0-numMines as a mine

		for (unsigned int i = 0; i < numMines; i++)
		{
			pair<int, int> combo = combos[i];
			tileVector[combo.first][combo.second].mine = true;
		}
	}

	else
	{
		width = 0;
		height = 0;
		numMines = 0;
		ifstream file(filename);
		string singleLine;

		int row = 0;
		int col = 0;
		while (getline(file, singleLine))
		{
			width = singleLine.size();
			for (unsigned int i = 0; i < width; i++)
			{
				if (singleLine[i] == '1')
				{
					numMines += 1;
				}
			}
			row += 1;
			height += 1;
		}

		tileVector.resize(height); // Creates a vector of all tiles in the game
		for (unsigned int i = 0; i < height; i++)
		{
			tileVector[i].resize(width);
		}

		file.clear();
		file.seekg(0);
		row = 0;

		while (getline(file, singleLine))
		{
			width = singleLine.size();
			for (unsigned int i = 0; i < width; i++)
			{
				if (singleLine[i] == '1')
				{
					tileVector[row][i].mine = true;
				}
			}
			row += 1;
		}
	}

	for (unsigned int i = 0; i < height; i++) // Iterates through each tile on the board and adds the neighbors to a vector
	{
		for (unsigned int j = 0; j < width; j++)
		{
			for (int k = -1; k <= 1; k++)
			{
				for (int l = -1; l <= 1; l++)
				{
					if (k == 0 && l == 0) // Makes sure tile does not check itself 
					{
						continue;
					}

					if (k != 0 or l != 0) // If tile is not checking itself
					{
						int row = i + k;
						int col = j + l;
						if (row >= 0 && col >= 0 && row < height && col < width) // Verify the bounds
						{
							tileVector[i][j].adjacentTiles.push_back(&tileVector[row][col]);
						}
					}
				}
			}
		}
	}

	for (unsigned int i = 0; i < tileVector.size(); i++) // Iterates through every neighbor tile and if it is a mine, increases count
	{
		for (unsigned int j = 0; j < width; j++)
		{
			for (unsigned int k = 0; k < tileVector[i][j].adjacentTiles.size(); k++)
			{
				if (tileVector[i][j].adjacentTiles[k]->mine)
				{
					tileVector[i][j].neighborMines += 1;
				}
			}
		}
	}
}

const int Board::getWidth() // Grabber for width
{
	return width;
}

const int Board::getHeight() // Grabber for height
{
	return height;
}

const vector<vector<Tile>>& Board::getTileVector() // Grabber for tileVector
{
	return tileVector;
}

void Board::flagTile(int row, int col) // Changes the bool value for a flagged tile
{
	if (tileVector[row][col].revealed == false)
	{
		tileVector[row][col].flagged = !tileVector[row][col].flagged;
		if (tileVector[row][col].flagged == true)
		{
			flaggedTiles += 1;
		}

		else if (tileVector[row][col].flagged == false)
		{
			flaggedTiles -= 1;
		}
	}
}

void Board::revealTile(Tile* tile) // Reveal the tile
{
	if (tile->mine == true)
	{
		tile->revealed = true;
		gameOver = true;
		for (unsigned int i = 0; i < tileVector.size(); i++)
		{
			for (unsigned int j = 0; j < tileVector[i].size(); j++)
			{
				if (tileVector[i][j].mine == true)
				{
					tileVector[i][j].revealed = true;
				}
			}
		}
	}

	else
	{
		tile->revealed = true;
		if (tile->neighborMines == 0)
		{
			for (int i = 0; i < tile->adjacentTiles.size(); i++)
			{
				if (tile->adjacentTiles[i]->revealed or tile->adjacentTiles[i]->flagged)
				{
					continue;
				}

				else
				{
					revealTile(tile->adjacentTiles[i]);
				}
			}
		}
	}
}

void Board::revealTile(int row, int col)
{
	revealTile(&tileVector[row][col]);
}

bool Board::isGameOver()
{
	return gameOver;
}

bool Board::isGameWon()
{
	if (gameOver)
	{
		return false;
	}

	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			if (tileVector[i][j].mine == false && tileVector[i][j].revealed == false && !gameOver)
			{
				return false;
			}

			else if (tileVector[i][j].mine == false && tileVector[i][j].revealed == true && !gameOver)
			{
				continue;
			}
		}
	}
	return true;
}

int Board::getFlaggedTiles()
{
	return flaggedTiles;
}

int Board::getNumMines()
{
	return numMines;
}

Board::Board()
{
	build();
}

